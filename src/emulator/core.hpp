#pragma once

#include <map>

/***************************************************************************
* virtual processor emulator implementation
***************************************************************************/

#include "device.hpp" // core is a device
#include "arch/isa.hpp"
#include "interruption.hpp"
#include "fpu.hpp"
#include "general_register.hpp"
#include "cpuid.hpp"
#include "performance_monitor.hpp"
#include "pic.hpp"
#include "random.hpp"
#include "register_stack.hpp"
#include "soft_exception.hpp"
#include "status_register.hpp"
#include "debug_register.hpp"
#include "translation_unit.hpp"
#include "instruction_cache.hpp"

namespace postrisc {

const size_t ICACHE_LINESIZE = 16 * sizeof(CBundle);
const size_t ICACHE_SIZE = 32 * 1024;

/***************************************************************************
* monitor identifier register format
*
* 3210987654321098765432109876543210987654321098765432109876543210
* --------rrrrrrrrrrrrrrrrrrrrrrrr------------------dddddd----aaav
*
***************************************************************************/
const u64 mask_monitor_reg = U64C( 0xff00ffffffffffff );

class CExeFileHeader;
class DecoderContext;

/***************************************************************************
* processor core config space
***************************************************************************/
class CoreConfigSpace {
friend class boost::serialization::access;
public:
    CoreConfigSpace();
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader       header;
    u64                     m_InterruptVector;
    u64                     m_TimeCmp;
    space_filler<u64, 10>   reserved;
};

static_assert(sizeof(CoreConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
static_assert(std::is_standard_layout<CoreConfigSpace>::value);

template<class Archive>
void
CoreConfigSpace::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(m_InterruptVector);
    archive & BOOST_SERIALIZATION_NVP(m_TimeCmp);
}

class CDebugLinkInfo {
public:
    CDebugLinkInfo(u64 _ip, u64 _sp, u64 _fp) : ip(_ip), sp(_sp), fp(_fp) {}

    u64 ip;
    u64 sp;
    u64 fp;
};

class CCoreInstructionInfo {
public:
    unsigned get_framesize(void) const { return rb; }

public:
    i64         imm;
    i64         brc_imm;
    unsigned    ra, rb, rc, rd, re; // virtual registers, shift amount
    unsigned    pra, prb, prc, prd; // physical registers
    unsigned    scale;              // indexed scale
    unsigned    dist_yes, dist_no;  // nullification distance
    union {
        unsigned      rounding_mode;
        EMemoryOrder  memory_order;
        unsigned      scale_mode;
    };
};

/***************************************************************************
* processor state
***************************************************************************/
class Core final : public Device {
friend class boost::serialization::access;
friend class emulator_test_fixture;  // security hole for unittests
template<class CMD> friend class CInsnHandler;

public:
    Core(const std::string& name, u32 core_id, unsigned flags);

    // Device
    ~Core(void) override;
    Core *get_bootstrap_core(void) override;

    STANDARD_DEVICE_INTERFACE

    void dump_state(const DumpFormatter& out) const;
    CStatus post_interrupt(CStatus vector_index);
    void reset(const unsigned coreid);
    void SetIp(const u64 pip);

    CStatus DisassembleBundle(IAddressSpace& as, std::ostream & out,
                              i64 offset, DecoderContext& decoderContext);
    CStatus PrepareThread(u64 sp, u64 bp, u64 tp, u64 eip);
    CStatus execute_until_interruption(IAddressSpace& as);

    bool IsStopped(void) const { return m_Stopped; }
    TranslationUnit & get_mmu() { return mmu; }

    void ResetCodeCache(void);
    void FlushTranslationBuffer(void) { mmu.FlushTranslationBuffer(); }
    void FlushTranslationBuffer(u64 address) { mmu.FlushTranslationBuffer(address); }

    template<class helper>
    std::tuple<CStatus, CPhysAddress>
    TranslateAddress(IAddressSpace &as, const CVirtAddress va,
                     u64 requested_rights, u64 unexpected_rights);

    const TranslationUnit& GetMMU(void) const { return mmu; }
    CStatus fetch_code(IAddressSpace &as, CPhysAddress pa, size_t len, void *data);
    u64 GetPerfInsn(void) const { return performance_monitor.GetPerfInsn(); }

private:
    ALWAYS_INLINE const CGeneralRegister & rget(unsigned phys_regnum) const;
    ALWAYS_INLINE       CGeneralRegister & wget(unsigned phys_regnum);

    ALWAYS_INLINE const CGeneralRegister & r_static_reg(unsigned static_reg) { return rget(rsc.RegStaticToPhys(static_reg)); }
    ALWAYS_INLINE       CGeneralRegister & w_static_reg(unsigned static_reg) { return wget(rsc.RegStaticToPhys(static_reg)); }

    void RaiseFault(const CStatus status);  // setup exception and retire
    void RaiseTrap(const CStatus status, const CBundle bundle);    // setup exception and NOT retire
    void TriggerEvent(CStatus event); // exception (common part of raise & trap)
    void DumpProfile(const DumpFormatter& out) const;
    void DumpInstructionFrequencies(const DumpFormatter& out) const;
    void SetupDebugPlace(u64 address, unsigned ri);

    CStatus ExecuteBundle(IAddressSpace& as, const CBundle bundle);
    template<class CMD> CStatus ExecuteInstruction(IAddressSpace& as, const CMD cmd);
    CStatus unimplemented(void) const;
    CStatus trigger_register_fault(unsigned rnum) const;
    template<class CMD> void report_fault(IAddressSpace& as, const CMD insn_cmd, CStatus status) const;

    static CStatus FinalizeInstruction();
    static CStatus FpuFinalizeInstruction();

    void SetFaultingAddress(u64 address) { ifa = address; }
    CStatus GetRoundingMode(unsigned roundmode, fpu::RoundingModeEnum & rm);
    CStatus ExecuteJump(IAddressSpace& as, u64 target);
    CStatus AllocateFrame(IAddressSpace& as, const unsigned regnum);
    CStatus OutputString(IAddressSpace& as, u64 address);
    CStatus OutputStringFormatting(IAddressSpace& as, std::ostream& stream, std::istream& input);
    CStatus GetSpecialRegister(unsigned int regnum, u64& out);
    CStatus SetSpecialRegister(unsigned int regnum, u64 value);
    CStatus OutputSpecialRegister(const DumpFormatter& out, const unsigned regnum) const;
    CStatus CallSubroutine(IAddressSpace& as, const unsigned preserve, CGeneralRegister & retreg, u64 address);
    CStatus ReturnFromSubroutine(IAddressSpace& as, const CGeneralRegister link);
    CStatus DoConditionalBranch(IAddressSpace& as, bool condition);
    CStatus NullifyConditionally(bool condition);

    template<class T, class OP> ALWAYS_INLINE CStatus NullifyConditionallyRegReg(void);
    template<class T, class OP> ALWAYS_INLINE CStatus NullifyConditionallyRegSimm(void);

    template<class T, class CMP, int step> ALWAYS_INLINE CStatus RepeatBranch(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus tabular_branch(IAddressSpace& as);

    template<class T, class OP> ALWAYS_INLINE CStatus binary_reg_reg(void);
    template<class T, class OP> ALWAYS_INLINE CStatus binary_reg_reg_divmod(void);
    template<class T, class OP> ALWAYS_INLINE CStatus BinaryRegRegBool(void);
    template<class T, class OP> ALWAYS_INLINE CStatus BinaryRegShamt(void);
    template<class T, class OP> ALWAYS_INLINE CStatus BinaryRegSimm(void);
    template<class T, class OP> ALWAYS_INLINE CStatus binary_reg_simm_divmod(void);
    template<class T, class OP> ALWAYS_INLINE CStatus fused_reg_reg_reg(void);
    template<class T, class OP> ALWAYS_INLINE CStatus fused_reg_reg_shamt(void);

    template<class T, class OP> ALWAYS_INLINE CStatus CompareRegRegAndBranch(IAddressSpace& as);
    template<class T, class OP> ALWAYS_INLINE CStatus CompareRegSimmAndBranch(IAddressSpace& as);

    template<class T, class OP> ALWAYS_INLINE CStatus CompareAndTrap(void);

    // load/store templates
    template<class T> ALWAYS_INLINE CStatus read_base(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus write_base(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus read_ip_relative(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus write_ip_relative(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus read_indexed(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus write_indexed(IAddressSpace& as);

    // load/store with post-update templates
    template<class T> ALWAYS_INLINE CStatus LoadModifyImmediateAfter(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus StoreModifyImmediateAfter(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus LoadModifyImmediateBefore(IAddressSpace& as);
    template<class T> ALWAYS_INLINE CStatus StoreModifyImmediateBefore(IAddressSpace& as);

    // atomic templates
    template<class T> CStatus atomic_load(IAddressSpace& as);
    template<class T> CStatus atomic_store(IAddressSpace& as);
    template<class T, class OP> CStatus fetch_and_op_atomic(IAddressSpace& as);
    template<class T, class OP> CStatus store_op_atomic(IAddressSpace& as);
    template<class T> CStatus compare_and_swap(IAddressSpace& as);

    // fpu/vector/simd templates
    template<class T> CStatus FpClassify(void);
    template<class T, class OP> CStatus FpBranchOnCompare(IAddressSpace& as);
    template<class T, class OP> CStatus FpNullifyOnCompare(void);
    template<class T> CStatus FpBranchOnClassify(IAddressSpace& as);
    template<class T> CStatus FpNullifyOnClassify(void);
    template<class T> CStatus SimdMergeLow(void);
    template<class T> CStatus SimdMergeHigh(void);

    template<class T, class WT> CStatus SimdUnpackLow(void);
    template<class T, class WT> CStatus SimdUnpackHigh(void);
    template<class T, class WT, class OP> CStatus VectorPack(void);

    template<class T, class OP> CStatus VectorBinaryRegReg(void);
    template<class T, class OP> CStatus VectorBinaryRegShamt(void);
    template<class T, class OP> CStatus VectorCompareRegReg(void);
    template<class T, class OP> CStatus FpScalarCompare(void);

    template<class D, class T, class OP> CStatus ScalarOp2(void);
    template<class D, class T, class OP> CStatus ScalarOp2RoundingMode(void);
    template<class D, class T, class OP> CStatus ScalarOp3(void);
    template<class D, class T, class OP> CStatus FpVectorCompare(void);
    template<class D, class T, class OP> CStatus ScalarOp4(void);
    template<class D, class T, class OP> CStatus VectorOp2(void);
    template<class D, class T, class OP> CStatus VectorOp2RoundingMode(void);
    template<class D, class T, class OP> CStatus VectorOp3(void);
    template<class D, class T, class OP> CStatus VectorOp3Horizontal(void);
    template<class D, class T> CStatus VectorOp3DotProduct(void);
    template<class D, class T, class OP> CStatus VectorOp4(void);

    template<class T> CStatus PackedMulAddAlternate(fpu::muladd_negate_t even_sig, fpu::muladd_negate_t odd_sig);
    template<class T> CStatus PackedAddSubAlternate(void);
    template<class T> CStatus PackedSubAddAlternate(void);

    template<class CMD> using insn_fmt_handler = CStatus (*)(Core& core, IAddressSpace& as, EInstructionID insn, const CMD cmd);

#define INSTRUCTION_FORMAT_PROTO_X(name, extname, descr, format, txt) \
    template<class CMD> static CStatus                                \
    insn_fmt_handler_##name(Core& core, IAddressSpace& as, EInstructionID insn, const CMD cmd);
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_PROTO_X)

#define DEFINE_INSN_FMT_IMPLEMENTATION(name) \
    template<class CMD> CStatus              \
    Core::insn_fmt_handler_##name(Core& core, IAddressSpace& as, EInstructionID insn, const CMD cmd)

    CGeneralRegister& ra(void) { return gpr[cmd.pra]; }
    CGeneralRegister& rb(void) { return gpr[cmd.prb]; }
    CGeneralRegister& rc(void) { return gpr[cmd.prc]; }
    CGeneralRegister& rd(void) { return gpr[cmd.prd]; }

    CGeneralRegister const& ra(void) const { return gpr[cmd.pra]; }
    CGeneralRegister const& rb(void) const { return gpr[cmd.prb]; }
    CGeneralRegister const& rc(void) const { return gpr[cmd.prc]; }
    CGeneralRegister const& rd(void) const { return gpr[cmd.prd]; }

    u64 ComputeScaledIndex(void) const;

public:
    static void dump_insn_stat(std::ostream& out, const std::array<u64, insn_reserved>& stat, char const *title);
    static CStatus insn_illegal(Core& core, IAddressSpace& as);
// prototypes for all instructions handlers
// static to optimize call-per-pointer sequence
#define X(name, subset, format, descr) static CStatus insn_##name(Core& core, IAddressSpace& as);
#include "arch/insn_table.hpp"
#undef X

private:
    template<class Archive> void serialize(Archive &ar, const unsigned int version);

private:
    CCoreInstructionInfo                            cmd;

    // core state
    CRegisterStackControlRegister                   rsc;     // register stack control
    u64                                             ip;      // instruction pointer
    fpu::CSoftFPU                                   fpu;     // floating-point control
    std::array<CGeneralRegister,
        num_static_regs + phys_stacked_regs>        gpr;     // general registers
    u64                                             iip;     // interrupt instruction pointer
    u64                                             iipa;    // interrupt instruction previous address
                                                             // (with ri (restart instruction) field in 2 lowest bits)
    u64                                             rsp;     // register stack pointer (bottom of dirty regs in memory)
    u64                                             peb;     // process env block
    u64                                             teb;     // thread env block
    u64                                             itc;     // interval time counter
    u64                                             itm;     // interval time match register
    CProcessorStatusRegister                        psr;     // processor status register
    CProcessorStatusRegister                        ipsr;    // processor status register, interruption copy
    CInterruptionCauseRegister                      cause;   // interruption cause register
    u64                                             kip;     // syscall, kernel instruction pointer
    u64                                             krsp;    // syscall, kernel register stack pointer
    u64                                             ksp;     // syscall, kernel stack pointer
    u64                                             iva;     // interruption vector address
    u64                                             ifa;     // interruption: faulting address
    CBundle                                         iib;     // interruption instruction bundle
    pic_t                                           pic;     // local programmable interrupt controller
    TranslationUnit                                 mmu;     // memory management unit
    CRandomUnit                                     random;  // random generator unit
    CpuIdentificationStore                          cpuid;   // cpuid read-only store
    SoftExceptionState                              exception_state; // exception instruction pointer (eip),
                                                                     // exception context (eca)
    std::array<CInstructionBreakpointRegister, 4>   ibr;
    std::array<CDataBreakpointRegister, 4>          dbr;
    CPerformanceMonitor                             performance_monitor;
    double                                          m_RunTime;
    std::map<u64, u32>                              m_profile;
    unsigned                                        m_config_flags;
    bool                                            m_Stopped;
    u64                                             debug_ip;
    unsigned                                        debug_ri;
    CoreConfigSpace                                 config;
    std::array<u64, ICACHE_SIZE / ICACHE_LINESIZE>  m_instruction_cache_tags;
    alignas(16) std::array<
    InstructionCacheLine<ICACHE_LINESIZE>,
           ICACHE_SIZE / ICACHE_LINESIZE>           m_instruction_cache_data;

private:
#if defined(DEBUG_LINK_INFO_STACK)
    CStatus CheckStackOnReturn(void);
    std::vector<CDebugLinkInfo>                     m_debug_link_info_stack;
#endif
#if defined(DEBUG_BACKSTORE_STACK)
    std::vector<CGeneralRegister>                   m_debug_backstore_stack;
#endif
    std::array<u64, insn_reserved>                  m_debug_instruction_stat;
};

#define DEFINE_INSN_IMPLEMENTATION(name) CStatus Core::insn_##name(Core& core, IAddressSpace& as)

#define DEFINE_INSN_IMPLEMENTATION_NOMEM(name) CStatus Core::insn_##name(Core& core, IAddressSpace& UNUSED(as))

template<class Archive>
void
Core::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";

    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);

    ar & BOOST_SERIALIZATION_NVP(rsc);
    ar & BOOST_SERIALIZATION_NVP(ip);
    ar & BOOST_SERIALIZATION_NVP(fpu);
    ar & BOOST_SERIALIZATION_NVP(iip);
    ar & BOOST_SERIALIZATION_NVP(iipa);
    ar & BOOST_SERIALIZATION_NVP(rsp);
    ar & BOOST_SERIALIZATION_NVP(peb);
    ar & BOOST_SERIALIZATION_NVP(teb);
    ar & BOOST_SERIALIZATION_NVP(itc);
    ar & BOOST_SERIALIZATION_NVP(itm);
    ar & BOOST_SERIALIZATION_NVP(psr);
    ar & BOOST_SERIALIZATION_NVP(ipsr);
    ar & BOOST_SERIALIZATION_NVP(cause);
    ar & BOOST_SERIALIZATION_NVP(kip);
    ar & BOOST_SERIALIZATION_NVP(krsp);
    ar & BOOST_SERIALIZATION_NVP(ksp);
    ar & BOOST_SERIALIZATION_NVP(iva);
    ar & BOOST_SERIALIZATION_NVP(ifa);
    ar & BOOST_SERIALIZATION_NVP(iib);
    ar & BOOST_SERIALIZATION_NVP(pic);
    ar & BOOST_SERIALIZATION_NVP(mmu);
    ar & BOOST_SERIALIZATION_NVP(random);
    ar & BOOST_SERIALIZATION_NVP(cpuid);
    ar & BOOST_SERIALIZATION_NVP(ibr);
    ar & BOOST_SERIALIZATION_NVP(dbr);
    ar & BOOST_SERIALIZATION_NVP(performance_monitor);
    ar & BOOST_SERIALIZATION_NVP(gpr);
    ar & BOOST_SERIALIZATION_NVP(m_RunTime);
    ar & BOOST_SERIALIZATION_NVP(m_profile);
    ar & BOOST_SERIALIZATION_NVP(m_Stopped);
    ar & BOOST_SERIALIZATION_NVP(debug_ip);
    ar & BOOST_SERIALIZATION_NVP(debug_ri);
    ar & BOOST_SERIALIZATION_NVP(m_instruction_cache_tags);
    ar & BOOST_SERIALIZATION_NVP(m_instruction_cache_data);
}

inline CStatus Core::FinalizeInstruction()
{
    return CStatus::continue_execution;
}

inline CStatus Core::FpuFinalizeInstruction()
{
    return FinalizeInstruction();
}

ALWAYS_INLINE CGeneralRegister & Core::wget(unsigned phys_regnum)
{
    CGeneralRegister & reg = gpr[ phys_regnum ];
    // LOG_DEBUG(LOG_REGISTER_DATA) << "before=" << phys_regnum << ":" << reg;
    return reg;
}

ALWAYS_INLINE const CGeneralRegister & Core::rget(unsigned phys_regnum) const
{
    const CGeneralRegister & reg = gpr[ phys_regnum ];
    // LOG_DEBUG(LOG_REGISTER_DATA) << "read=" << phys_regnum << ":" << reg;
    return reg;
}

#define STRUCT_MEMBER_SIZE(struct_, member_)  sizeof(((struct_ *)nullptr) -> member_)

/***************************************************************************
* register windows/rotation macros
* used inside core class member functions only
***************************************************************************/

#define ROTATE_REG(core, DST, TEMP, REGSOURCE)                      \
    TEMP = REGSOURCE;                                               \
    if (likely(is_rotate_reg(TEMP))) {                              \
        if (unlikely((core).rsc.IsOutOfFrame(TEMP))) {              \
            LOG_ERROR(LOG_REGISTER_ROTATION | LOG_EXCEPTION)        \
                << "rotate reg=" << TEMP                            \
                << " SizeOfFrame=" << (core).rsc.GetSizeOfFrame();  \
            return CStatus::register_window_protection;             \
        }                                                           \
        DST = (core).rsc.rotate(TEMP);                              \
    } else {                                                        \
        DST = (core).rsc.RegStaticToPhys(TEMP);                     \
    }                                                               \
    LOG_DEBUG(LOG_REGISTER_DATA)                                    \
        << "reg:" << TEMP                                           \
        << ":" << DST                                               \
        << ":" << (core).gpr[DST];                                  \


#define CHECK_KERNEL_MODE(core)                                            \
    if (unlikely(!core.psr.IsKernelMode())) {                              \
        LOG_ERROR(LOG_EXCEPTION) << "privileged instruction";              \
        return CStatus::general_protection;                                \
    }

#define CHECK_STATUS_ADDRESS(core, va, status)                             \
    do {                                                                   \
        CStatus status__ = (status);                                       \
        if (unlikely(!status__.continuable())) {                           \
            (core).SetFaultingAddress(va.get_address());                   \
            return status__;                                               \
        }                                                                  \
    } while (0)


ALWAYS_INLINE u64 Core::ComputeScaledIndex(void) const
{
    u64 index = rc().base();
    if (cmd.scale_mode & scale_mode_word) {
        if (cmd.scale_mode & scale_mode_signed) {
           index = (i64)static_cast<i32>(index);
        } else {
           index = (u64)static_cast<u32>(index);
        }
    }
    return rb().base() + (index << cmd.scale) + cmd.imm;
}

ALWAYS_INLINE CStatus Core::ExecuteJump(IAddressSpace& as, u64 target)
{
    LOG_DEBUG(LOG_BRANCH)
        << "ip=" << fmt::addr_info(as.get_decoder(), ip)
        << " target=" << fmt::addr_info(as.get_decoder(), target);

    // save branch source info in iipa
    iipa = (ip - sizeof(CBundle)) | psr.GetShadowSize();

    // avoid trap-handler shadow setup for incomplete branch
    psr.ClearShadow();

    // clear possible future predication mask
    psr.ClearFuture();

    ip = target;

    return CStatus::taken_branch;
}

template<class helper>
ALWAYS_INLINE
std::tuple<CStatus, CPhysAddress>
Core::TranslateAddress(IAddressSpace &as, const CVirtAddress va,
                       u64 requested_rights, u64 unexpected_rights)
{
    return mmu.TranslateAddress<helper>(as, performance_monitor,
               va, requested_rights, unexpected_rights);
}

ALWAYS_INLINE
CStatus
Core::fetch_code(IAddressSpace &as, CPhysAddress pa, size_t len, void *data)
{
    assert(len == ICACHE_LINESIZE);

    // search instruction cache
    u64 const phys_address = pa.get_address();
    u64 const tag = phys_address / ICACHE_LINESIZE;
    size_t const index = tag % m_instruction_cache_data.size();

    u64 const cache_address =
        util::align_down<u64>(phys_address, ICACHE_LINESIZE);

    if (unlikely(m_instruction_cache_tags[index] != cache_address)) {
        // icache miss, refill
        performance_monitor.report_event(PMC_ICACHE_MISS);
        LOG_DEBUG(LOG_ICACHE) << "icache miss: " << fmt::hex(cache_address);
        CHECK_STATUS(as.bus_read(CPhysAddress(cache_address),
                          sizeof(m_instruction_cache_data[index]),
                          &m_instruction_cache_data[index]));
        m_instruction_cache_tags[index] = cache_address;
    } else {
        performance_monitor.report_event(PMC_ICACHE_HIT);
        LOG_DEBUG(LOG_ICACHE) << "icache hit: " << fmt::hex(cache_address);
    }

    memcpy(data, &m_instruction_cache_data[index], len);
    return CStatus::continue_execution;
}

} // namespace postrisc
