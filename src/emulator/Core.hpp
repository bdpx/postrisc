#pragma once

#include <map>

#include "arch/isa.hpp"
#include "Device.hpp"
#include "GeneralRegister.hpp"
#include "CpuIdentificationStore.hpp"
#include "PerformanceMonitor.hpp"
#include "ProgrammableInterruptController.hpp"
#include "RandomUnit.hpp"
#include "RegisterStackControlRegister.hpp"
#include "SoftExceptionState.hpp"
#include "ProcessorStatusRegister.hpp"
#include "InstructionBreakpointRegister.hpp"
#include "DataBreakpointRegister.hpp"
#include "TranslationUnit.hpp"
#include "InstructionCache.hpp"
#include "SoftFpu.hpp"
#include "InstructionStatisticCollector.hpp"

namespace postrisc {

const size_t ICACHE_LINESIZE = 16 * sizeof(Bundle);
const size_t ICACHE_SIZE = 32 * 1024;

/***************************************************************************
* monitor identifier register format
*
* 3210987654321098765432109876543210987654321098765432109876543210
* --------rrrrrrrrrrrrrrrrrrrrrrrr------------------dddddd----aaav
*
***************************************************************************/
const u64 mask_monitor_reg = U64C( 0xff00ffffffffffff );

class DecoderContext;

/***************************************************************************
* processor core config space
***************************************************************************/
class CoreConfigSpace {
friend class boost::serialization::access;
public:
    CoreConfigSpace();
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader       header;
    u64                     m_InterruptVector;
    u64                     m_TimeCmp;
    space_filler<u64, 10>   reserved;
};

static_assert(sizeof(CoreConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
static_assert(std::is_standard_layout<CoreConfigSpace>::value);

template<typename Archive>
void
CoreConfigSpace::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(m_InterruptVector);
    archive & BOOST_SERIALIZATION_NVP(m_TimeCmp);
}

class DebugLinkInfo {
public:
    DebugLinkInfo(u64 _ip, u64 _sp, u64 _fp) : ip(_ip), sp(_sp), fp(_fp) {}

    u64 ip;
    u64 sp;
    u64 fp;
};

class CoreInstructionInfo {
public:
    unsigned get_framesize(void) const { return rb; }

public:
    i64         imm;
    i64         brc_imm;
    unsigned    ra, rb, rc, rd, re; // virtual registers, shift amount
    unsigned    pra, prb, prc, prd; // physical registers
    unsigned    scale;              // indexed scale
    unsigned    dist_yes, dist_no;  // nullification distance
    unsigned    num_slots;
};

/***************************************************************************
* processor state
***************************************************************************/
class Core final : public Device {
friend class boost::serialization::access;
friend class emulator_test_fixture;  // security hole for unittests

public:
    Core(const std::string& name, u32 core_id, unsigned flags);

    Core(Core&) = delete;
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    // Device
    ~Core(void) override;
    Core *get_bootstrap_core(void) override;

    STANDARD_DEVICE_INTERFACE

    void dump_state(const DumpFormatter& out) const;
    Result post_interrupt(Result vector_index);
    void reset(const unsigned coreid);
    void SetIp(const u64 pip);

    void DisassembleDynamic(AddressSpace& as, std::ostream& out, i64 start, i64 stop);
    Result PrepareThread(u64 bsp, u64 sp, u64 bp, u64 tp, u64 eip);
    Result execute_until_interruption(AddressSpace& as);

    bool IsStopped(void) const { return m_Stopped; }
    TranslationUnit & get_mmu() { return mmu; }

    void ResetCodeCache(void);
    void FlushTranslationBuffer(void) { mmu.FlushTranslationBuffer(); }
    void FlushTranslationBuffer(u64 address) { mmu.FlushTranslationBuffer(address); }

    template<typename helper>
    std::tuple<Result, PhysAddress>
    TranslateAddress(const AddressSpace &as, const VirtAddress va,
                     u64 requested_rights, u64 unexpected_rights);

    const TranslationUnit& GetMMU(void) const { return mmu; }
    Result fetch_code(AddressSpace &as, PhysAddress pa, size_t len, void *data);
    u64 GetPerfInsn(void) const { return performance_monitor.GetPerfInsn(); }
    u64 GetPerfTime(void) const { return performance_monitor.GetPerfTime(); }
    void set_itc(u64 instruction_limit) { itc = instruction_limit; }

private:
    ALWAYS_INLINE const GeneralRegister & rget(unsigned phys_regnum) const;
    ALWAYS_INLINE       GeneralRegister & wget(unsigned phys_regnum);

    ALWAYS_INLINE const GeneralRegister & r_static_reg(unsigned static_reg) { return rget(rsc.RegStaticToPhys(static_reg)); }
    ALWAYS_INLINE       GeneralRegister & w_static_reg(unsigned static_reg) { return wget(rsc.RegStaticToPhys(static_reg)); }

    void DumpProfile(const DumpFormatter& out) const;
    void SetupDebugPlace(u64 address, unsigned restarting_slot);

    Result ExecuteBundle(AddressSpace& as, const Bundle bundle);
    template<typename CMD> Result ExecuteInstruction(AddressSpace& as, const CMD cmd);
    Result unimplemented(void) const;
    Result trigger_register_fault(unsigned rnum) const;
    template<typename CMD> void report_fault(AddressSpace& as, const CMD insn_cmd, Result result) const;

    static Result FinalizeInstruction();
    static Result FpuFinalizeInstruction();
    Result Raise(Result error);

    void SetFaultingAddress(u64 address) { ifa = address; }
    Result ExecuteJump(AddressSpace& as, u64 target);
    Result AllocateFrame(AddressSpace& as, const unsigned regnum);
    Result OutputString(AddressSpace& as, u64 address);
    Result OutputStringFormatting(AddressSpace& as, std::ostream& stream, std::istream& input);
    Result GetSpecialRegister(unsigned int regnum, GeneralRegister& out);
    Result SetSpecialRegister(unsigned int regnum, GeneralRegister value);
    Result OutputSpecialRegister(const DumpFormatter& out, const unsigned regnum) const;
    Result CallSubroutine(AddressSpace& as, const unsigned preserve, GeneralRegister & retreg, u64 address);
    Result ReturnFromSubroutine(AddressSpace& as, const GeneralRegister link);
    Result DoConditionalBranch(AddressSpace& as, bool condition);
    Result NullifyConditionally(bool condition);

    template<typename T, typename OP> ALWAYS_INLINE Result NullifyConditionallyRegReg(void);
    template<typename T, typename OP> ALWAYS_INLINE Result NullifyConditionallyRegSimm(void);

    template<typename T, typename CMP, typename OP> ALWAYS_INLINE Result RepeatBranch(AddressSpace& as);

    template<typename T> ALWAYS_INLINE Result tabular_branch(AddressSpace& as);

    template<typename T, typename OP> ALWAYS_INLINE Result unary_reg(void);
    template<typename T, typename OP> ALWAYS_INLINE Result binary_reg_reg(void);
    template<typename T, typename OP> ALWAYS_INLINE Result binary_reg_reg_divmod(void);
    template<typename T, typename OP> ALWAYS_INLINE Result BinaryRegRegBool(void);
    template<typename T, typename OP> ALWAYS_INLINE Result BinaryRegShamt(void);
    template<typename T, typename OP> ALWAYS_INLINE Result BinaryRegSimm(void);
    template<typename T, typename OP> ALWAYS_INLINE Result binary_reg_simm_divmod(void);
    template<typename T, typename OP> ALWAYS_INLINE Result fused_reg_reg_reg(void);
    template<typename T, typename OP> ALWAYS_INLINE Result fused_reg_reg_shamt(void);
    template<typename T, typename OP> ALWAYS_INLINE Result ConditionalMove(void);

    template<typename T, typename OP> ALWAYS_INLINE Result CompareRegRegAndBranch(AddressSpace& as);
    template<typename T, typename OP> ALWAYS_INLINE Result CompareRegSimmAndBranch(AddressSpace& as);

    template<typename T, typename OP> ALWAYS_INLINE Result CompareAndTrap(void);

    // base+displacement load/store templates
    template<typename T> ALWAYS_INLINE Result read_base(AddressSpace& as);
    template<typename T> ALWAYS_INLINE Result write_base(AddressSpace& as);

    // ip-reative load/store templates
    template<typename T> ALWAYS_INLINE Result read_ip_relative(AddressSpace& as);
    template<typename T> ALWAYS_INLINE Result write_ip_relative(AddressSpace& as);

    template<typename INDEX> u64 ComputeIndexingAddress(void) const;
    template<typename INDEX> ALWAYS_INLINE Result compute_address_indexed(AddressSpace& as);
    template<typename T, typename INDEX> ALWAYS_INLINE Result read_indexed(AddressSpace& as);
    template<typename T, typename INDEX> ALWAYS_INLINE Result write_indexed(AddressSpace& as);

    // load/store with pre|post-update templates
    template<typename T> ALWAYS_INLINE Result LoadModifyImmediateAfter(AddressSpace& as);
    template<typename T> ALWAYS_INLINE Result StoreModifyImmediateAfter(AddressSpace& as);
    template<typename T> ALWAYS_INLINE Result LoadModifyImmediateBefore(AddressSpace& as);
    template<typename T> ALWAYS_INLINE Result StoreModifyImmediateBefore(AddressSpace& as);

    // atomic templates
    template<typename T> Result atomic_load(AddressSpace& as, std::memory_order);
    template<typename T> Result atomic_store(AddressSpace& as, std::memory_order);
    template<typename T, typename STORE_T, typename OP> Result fetch_and_op_atomic(AddressSpace& as, std::memory_order order);
    template<typename T, typename OP> Result store_op_atomic(AddressSpace& as, std::memory_order order);
    template<typename T> Result compare_and_swap(AddressSpace& as, std::memory_order order);
    Result atomic_fence(AddressSpace& as, std::memory_order);

    // fpu/vector/simd templates
    template<typename T> Result FpClassify(void);
    template<typename T, typename OP> Result FpBranchOnCompare(AddressSpace& as);
    template<typename T, typename OP> Result FpNullifyOnCompare(void);
    template<typename T> Result SimdMergeLow(void);
    template<typename T> Result SimdMergeHigh(void);

    template<typename T, typename WT> Result SimdUnpackLow(void);
    template<typename T, typename WT> Result SimdUnpackHigh(void);
    template<typename T, typename WT, typename OP> Result VectorPack(void);

    template<typename T, typename OP> Result VectorBinaryRegReg(void);
    template<typename T, typename OP> Result VectorBinaryRegShamt(void);
    template<typename T, typename OP> Result VectorCompareRegReg(void);
    template<typename T, typename OP> Result FpScalarCompare(void);

    template<typename D, typename T, typename OP> Result ScalarOp2(void);
    template<typename D, typename T, typename OP> Result ScalarOp2RoundingMode(fpu::RoundingModeEnum rm);
    template<typename D, typename T, typename OP> Result ScalarOp3(void);
    template<typename D, typename T, typename OP> Result FpVectorCompare(void);
    template<typename D, typename T, typename OP> Result ScalarOp4(void);
    template<typename D, typename T, typename OP> Result VectorOp2(void);
    template<typename D, typename T, typename OP> Result VectorOp2RoundingMode(fpu::RoundingModeEnum rm);
    template<typename D, typename T, typename OP> Result VectorOp3(void);
    template<typename D, typename T, typename OP> Result VectorOp3Horizontal(void);
    template<typename D, typename T> Result VectorOp3DotProduct(void);
    template<typename D, typename T, typename OP> Result VectorOp4(void);

    template<typename CMD> using insn_fmt_handler = Result (*)(Core& core, AddressSpace& as, EInstructionID insn, const CMD cmd);

#define INSTRUCTION_FORMAT_PROTO_X(name, extname, descr, format, txt) \
    template<typename CMD> static Result insn_fmt_handler_##name(Core& core, AddressSpace& as, EInstructionID insn, const CMD cmd);

    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_PROTO_X)

#define DEFINE_INSN_FMT_IMPLEMENTATION(name) \
    template<typename CMD> Result Core::insn_fmt_handler_##name(Core& core, AddressSpace& as, EInstructionID insn, const CMD cmd)

    GeneralRegister& ra(void) { return gpr[cmd.pra]; }
    GeneralRegister& rb(void) { return gpr[cmd.prb]; }
    GeneralRegister& rc(void) { return gpr[cmd.prc]; }
    GeneralRegister& rd(void) { return gpr[cmd.prd]; }

    GeneralRegister const& ra(void) const { return gpr[cmd.pra]; }
    GeneralRegister const& rb(void) const { return gpr[cmd.prb]; }
    GeneralRegister const& rc(void) const { return gpr[cmd.prc]; }
    GeneralRegister const& rd(void) const { return gpr[cmd.prd]; }

public:
    static Result insn_illegal(Core& core, AddressSpace& as);
// prototypes for all instructions handlers
// static to optimize call-per-pointer sequence
#define X(name, subset, format, descr) static Result insn_##name(Core& core, AddressSpace& as);
#include "arch/insn_table.hpp"
#undef X

private:
    template<typename Archive> void serialize(Archive &ar, const unsigned int version);

private:
    CoreInstructionInfo                             cmd{};

    // core state
    RegisterStackControlRegister                    rsc{};   // register stack control
    u64                                             ip = 0;  // instruction pointer
    fpu::SoftFPU                                    fpu;     // floating-point control

    std::array<GeneralRegister, num_static_regs + phys_stacked_regs>  gpr;     // general registers

    u64                                             iip = 0; // interrupt instruction pointer
    u64                                             iipa = 0;// interrupt instruction previous address
                                                             // (with rs (restarting slot) field in 2 lowest bits)
    u64                                             rsp = 0; // register stack pointer (bottom of dirty regs in memory)
    u64                                             bsp = 0; // bottom stack pointer
    u64                                             peb = 0; // process env block
    u64                                             teb = 0; // thread env block
    u64                                             itc = 0; // interval time counter
    u64                                             itm = 0; // interval time match register
    ProcessorStatusRegister                         psr{};   // processor status register
    ProcessorStatusRegister                         ipsr{};  // processor status register, interruption copy
    CInterruptionCauseRegister                      cause;   // interruption cause register
    u64                                             kip = 0; // syscall, kernel instruction pointer
    u64                                             krsp = 0;// syscall, kernel register stack pointer
    u64                                             ksp = 0; // syscall, kernel stack pointer
    u64                                             iva = 0; // interruption vector address, need some default?
    u64                                             ifa = 0; // interruption: faulting address
    Bundle                                          iib{};   // interruption instruction bundle
    ProgrammableInterruptController                 pic{};   // local programmable interrupt controller
    TranslationUnit                                 mmu{};   // memory management unit
    RandomUnit                                      random;  // random generator unit
    CpuIdentificationStore                          cpuid{}; // cpuid read-only store
    SoftExceptionState                              exception_state; // exception instruction pointer (eip), exception context (eca)
    std::array<InstructionBreakpointRegister, 4>    ibr{};
    std::array<DataBreakpointRegister, 4>           dbr{};

    u64                                             fetched_bundles = 0;

public:
    PerformanceMonitor                              performance_monitor{};

private:
    double                                          m_RunTime = 0.0;
    std::map<u64, u32>                              m_profile;
    unsigned                                        m_config_flags = 0;
    bool                                            m_Stopped = false;
    u64                                             debug_ip = 0;
    unsigned                                        debug_restart_slot = 0;
    CoreConfigSpace                                 config{};
    std::array<u64, ICACHE_SIZE / ICACHE_LINESIZE>  m_instruction_cache_tags;

    std::array<InstructionCacheLine<ICACHE_LINESIZE>, ICACHE_SIZE / ICACHE_LINESIZE>   m_instruction_cache_data;

private:
    InstructionStatisticCollector                   m_instructionStatisticCollector;

#if defined(DEBUG_LINK_INFO_STACK)
    Result CheckStackOnReturn(void);
    std::vector<DebugLinkInfo>                      m_debug_link_info_stack;
#endif
#if defined(DEBUG_BACKSTORE_STACK)
    std::vector<GeneralRegister>                    m_debug_backstore_stack;
#endif
};

#define DEFINE_INSN_IMPLEMENTATION(name) Result Core::insn_##name(Core& core, AddressSpace& as)

#define DEFINE_INSN_IMPLEMENTATION_NOMEM(name) Result Core::insn_##name(Core& core, AddressSpace& UNUSED(as))

template<typename Archive>
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
    ar & BOOST_SERIALIZATION_NVP(bsp);
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
    ar & BOOST_SERIALIZATION_NVP(debug_restart_slot);
    ar & BOOST_SERIALIZATION_NVP(m_instruction_cache_tags);
    ar & BOOST_SERIALIZATION_NVP(m_instruction_cache_data);
}

inline Result Core::FinalizeInstruction()
{
    return Result::continue_execution;
}

inline Result Core::FpuFinalizeInstruction()
{
    return FinalizeInstruction();
}

ALWAYS_INLINE GeneralRegister & Core::wget(unsigned phys_regnum)
{
    GeneralRegister & reg = gpr[ phys_regnum ];
    // LOG_DEBUG(LOG_REGISTER_DATA) << "before=" << phys_regnum << ":" << reg;
    return reg;
}

ALWAYS_INLINE const GeneralRegister & Core::rget(unsigned phys_regnum) const
{
    const GeneralRegister & reg = gpr[ phys_regnum ];
    // LOG_DEBUG(LOG_REGISTER_DATA) << "read=" << phys_regnum << ":" << reg;
    return reg;
}

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
            return Result::register_window_protection;              \
        }                                                           \
        DST = (core).rsc.rotate(TEMP);                              \
    } else {                                                        \
        DST = (core).rsc.RegStaticToPhys(TEMP);                     \
    }                                                               \
    LOG_DEBUG(LOG_REGISTER_DATA)                                    \
        << "reg:" << TEMP                                           \
        << ":" << DST                                               \
        << ":" << (core).gpr[DST];                                  \


#define STATIC_ROTATE_REG(NAME)                                     \
    const unsigned r##NAME = cmd.r##NAME();                         \
    core.cmd.r##NAME = r##NAME;                                     \
    if (likely(is_rotate_reg(r##NAME))) {                           \
        if (unlikely(core.rsc.IsOutOfFrame(r##NAME))) {             \
            return core.trigger_register_fault(r##NAME);            \
        }                                                           \
        core.cmd.pr##NAME = core.rsc.rotate(r##NAME);               \
    } else {                                                        \
        core.cmd.pr##NAME = core.rsc.RegStaticToPhys(r##NAME);      \
    }                                                               \
    LOG_DEBUG(LOG_REGISTER_DATA)                                    \
        << "reg:" << r##NAME                                        \
        << ":" << core.cmd.pr##NAME                                 \
        << ":" << core.gpr[core.cmd.pr##NAME];                      \


#define CHECK_KERNEL_MODE(core)                                     \
    if (unlikely(!core.psr.IsKernelMode())) {                       \
        LOG_ERROR(LOG_EXCEPTION) << "privileged instruction";       \
        return Result::general_protection;                          \
    }

#define CHECK_RESULT_ADDRESS(core, va, result)                      \
    do {                                                            \
        const Result result__ = (result);                           \
        if (unlikely(!result__.continuable())) {                    \
            (core).SetFaultingAddress(va.get_address());            \
            return result__;                                        \
        }                                                           \
    } while (0)


template<typename INDEX>
ALWAYS_INLINE u64 Core::ComputeIndexingAddress(void) const
{
    const u64 index = rc().base();

    if constexpr (std::is_signed<INDEX>::value) {
        // signed indexing: base + scale * sext(index) + disp
        const i64 temp = static_cast<i64>(index);
        return rb().base() + (temp << cmd.scale) + cmd.imm;
    } else {
        // unsigned indexing: base + scale * zext(index + disp)
        const INDEX temp = static_cast<INDEX>(index + cmd.imm);
        return rb().base() + (temp << cmd.scale);
    }
}

ALWAYS_INLINE Result Core::ExecuteJump(AddressSpace& as, u64 target)
{
    LOG_DEBUG(LOG_BRANCH)
        << "ip=" << as.get_decoder().get_address_info(ip)
        << " target=" << as.get_decoder().get_address_info(target);

    // save branch source info in iipa
    iipa = ip | psr.rs.get();

    // avoid bundle partial restarting
    psr.rs.clear();

    psr.future.clear();

    ip = target;

    return Result::taken_branch;
}

template<typename helper>
ALWAYS_INLINE
std::tuple<Result, PhysAddress>
Core::TranslateAddress(const AddressSpace &as, const VirtAddress va, u64 requested_rights, u64 unexpected_rights)
{
    return mmu.TranslateAddress<helper>(as, performance_monitor, va, requested_rights, unexpected_rights);
}

ALWAYS_INLINE
Result
Core::fetch_code(AddressSpace &as, PhysAddress pa, size_t len, void *data)
{
    assert(len == ICACHE_LINESIZE);

    // search instruction cache
    u64 const phys_address = pa.get_address();
    u64 const tag = phys_address / ICACHE_LINESIZE;
    size_t const index = tag % m_instruction_cache_data.size();

    u64 const cache_address =
        util::rounddown_pow2<u64>(phys_address, ICACHE_LINESIZE);

    if (unlikely(m_instruction_cache_tags[index] != cache_address)) {
        // icache miss, refill
        performance_monitor.report_event(PMC_ICACHE_MISS);
        LOG_DEBUG(LOG_ICACHE) << "icache miss: " << fmt::hex(cache_address);
        CHECK_RESULT(as.bus_read(PhysAddress(cache_address),
                          sizeof(m_instruction_cache_data[index]),
                          &m_instruction_cache_data[index]));
        m_instruction_cache_tags[index] = cache_address;
    } else {
        performance_monitor.report_event(PMC_ICACHE_HIT);
        LOG_DEBUG(LOG_ICACHE) << "icache hit: " << fmt::hex(cache_address);
    }

    memcpy(data, &m_instruction_cache_data[index], len);
    return Result::continue_execution;
}

} // namespace postrisc
