#pragma once

#include <map>

/***************************************************************************
* virtual processor emulator implementation
***************************************************************************/

#include "arch/interruption.hpp"
#include "fpu/float.hpp"

#include "units/general_register.hpp"
#include "units/pic.hpp"
#include "units/random.hpp"
#include "units/cpuid.hpp"
#include "units/register_stack.hpp"
#include "units/soft_exception.hpp"
#include "units/status_register.hpp"
#include "units/debug_register.hpp"
#include "units/performance_monitor.hpp"
#include "units/translation_unit.hpp"

namespace postrisc {

using postrisc::fpu::u128;

const size_t ICACHE_LINESIZE = 16 * sizeof(CBundle);
const size_t ICACHE_SIZE = 32 * 1024;

/***************************************************************************
* monitor identifier register format
*
* 3210987654321098765432109876543210987654321098765432109876543210
* --------rrrrrrrrrrrrrrrrrrrrrrrr------------------dddddd----aaav
*
***************************************************************************/
const uint64_t mask_monitor_reg = UINT64_C( 0xff00ffffffffffff );

class CExeFileHeader;

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
    ConfigSpaceHeader            header;
    uint64_t                     m_InterruptVector;
    uint64_t                     m_TimeCmp;
    space_filler<uint64_t, 10>   reserved;
};

STATIC_ASSERT(sizeof(CoreConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
STATIC_ASSERT(std::is_standard_layout<CoreConfigSpace>::value);

template<class Archive>
void
CoreConfigSpace::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(m_InterruptVector);
    archive & BOOST_SERIALIZATION_NVP(m_TimeCmp);
}

/***************************************************************************
* processor state
***************************************************************************/
class Core : public Device {
friend class boost::serialization::access;
friend class emulator_test_fixture;  // security hole for unittests
template<class CMD> friend class CInsnHandler;

public:
    Core(TargetSystem *targetSystem, const std::string& name, uint32_t core_id, unsigned flags);

    // Device
    ~Core(void) override;
    Core *get_bootstrap_core(void) override;
    CStatus read(device_address_t addr, size_t len, void* bytes) override;
    CStatus write(device_address_t addr, size_t len, const void* bytes) override;
    CStatus read_config(device_address_t addr, size_t len, void *bytes) override;
    CStatus write_config(device_address_t addr, size_t len, const void *bytes) override;
    CStatus complex_operation(device_address_t addr, IMemoryOperation& op) override;
    uint64_t size(void) const override;
    uint64_t get_config_size(void) const override;
    void dump_state(const DumpFormatter& out) const;
    void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const override;
    void dump_config(uint64_t address, const DumpFormatter& out) const override;
    void dump_statistic(const DumpFormatter& out) const override;

    CStatus post_interrupt(CStatus vector_index);
    void reset(const unsigned coreid);
    void SetIp(const uint64_t pip);
    CStatus disassemble_bundle(std::ostream & out, int64_t offset, disassembler_mode mode);
    CStatus execute_until_interruption(void);

    bool IsStopped(void) const { return m_Stopped; }
    TranslationUnit & get_mmu() { return mmu; }

    // for TargetSystem
    CStatus PrepareStack(uint64_t vbase, size_t stack_size);
    CStatus PrepareBackingStore(uint64_t vbase, size_t rstack_size);
    CStatus PrepareThreadStorage(uint64_t vbase, size_t size);
    Decoder const& GetDecoder(void) const { return get_target_system()->GetDecoder(); }

    void ResetCodeCache(void);
    void FlushTranslationBuffer(void);
    void FlushTranslationBuffer(uint64_t address);
    CStatus dump_virt_map(const DumpFormatter& out) const;

    CStatus mmu_fetch_code(uint64_t address, InstructionCacheLine<ICACHE_LINESIZE> &);
    CStatus mmu_read(uint64_t virt_address, size_t len, void * data);
    CStatus mmu_read_finalized(uint64_t address, size_t len, void * data);
    CStatus mmu_read_backing_store(uint64_t address, size_t len, void * data);
    CStatus mmu_write(uint64_t virt_address, size_t len, const void * data);
    CStatus mmu_write_backing_store(uint64_t address, size_t len, const void * data);
    CStatus mmu_complex_operation(uint64_t address, uint64_t requested_rights, IMemoryOperation& op);

    CStatus VirtualMap(uint64_t base, phys_address_t phys_address, uint64_t size, uint64_t attributes);
    CStatus VirtualAllocate(uint64_t base, uint64_t size, uint64_t attributes);
    CStatus VirtualProtect(uint64_t base, uint64_t size, uint64_t attributes);
    CStatus VirtualFree(uint64_t base, uint64_t size);
    CStatus LoadSegment(uint64_t base, size_t size, const uint8_t *data, uint64_t final_rights);
    CStatus LoadModule(const uint64_t vbase, std::istream & input, CExeFileHeader & header);
    void RunMemoryTests(std::ostream & os);

private:
    ALWAYS_INLINE const CGeneralRegister & rget(unsigned phys_regnum) const;
    ALWAYS_INLINE CGeneralRegister & wget(unsigned phys_regnum);
    void RaiseFault(const CStatus status);  // setup exception and retire
    void RaiseTrap(const CStatus status, const CBundle bundle);    // setup exception and NOT retire
    void TriggerEvent(CStatus event); // exception (common part of raise & trap)
    void DumpProfile(const DumpFormatter& out) const;

    CStatus execute_bundle(const CBundle bundle);

    static CStatus TranslateLoadMemoryOrder(const EMemoryOrdering memorder, std::memory_order & stdorder);
    static CStatus TranslateStoreMemoryOrder(const EMemoryOrdering memorder, std::memory_order & stdorder);
    static CStatus TranslateMemoryOrder(const EMemoryOrdering memorder, std::memory_order & success_order, std::memory_order & fail_order);
    static CStatus FinalizeInstruction();
    static CStatus FpuFinalizeInstruction();

    void SetFaultingAddress(uint64_t address) { ifa = address; }
    CStatus GetRoundingMode(unsigned roundmode, fpu::rounding_mode_t & rm);
    CStatus ExecuteJump(uint64_t target);
    CStatus AllocateFrame(const unsigned regnum);
    CStatus OutputString(uint64_t address);
    CStatus OutputStringFormatting(std::ostream& stream, std::istream& input);
    CStatus GetSpecialRegister(unsigned int regnum, CGeneralRegister & out);
    CStatus SetSpecialRegister(unsigned int regnum, CGeneralRegister value);
    CStatus OutputSpecialRegister(const DumpFormatter& out, const unsigned regnum) const;
    CStatus CallSubroutine(const unsigned preserve, CGeneralRegister & retreg, uint64_t address);
    CStatus ReturnFromSubroutine(const CGeneralRegister & r, const unsigned disp);

    template<class CMD> CStatus execute_instruction(const CMD cmd);
    template<class CMD> CStatus unimplemented(const CMD cmd);

    template<class CMD> ALWAYS_INLINE CStatus DoConditionalBranch(const CMD cmd, bool condition);
    template<class CMD> ALWAYS_INLINE CStatus NullifyConditionally(const CMD cmd, bool condition);

    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus NullifyConditionallyRegReg(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus NullifyConditionallyRegSimm(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus NullifyConditionallyRegUimm(const CMD cmd);

    template<class T, class CMP, class CMD> ALWAYS_INLINE CStatus RepeatBranch(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus tabular_branch(const CMD cmd);

    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus binary_reg_reg(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus binary_reg_reg_fallible(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus BinaryRegRegBool(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus BinaryRegShamt(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus BinaryRegUimm(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus BinaryRegSimm(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus binary_reg_uimm_fallible(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus binary_reg_simm_fallible(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus fused_reg_reg_reg(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus fused_reg_reg_shamt(const CMD cmd);

    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus CompareRegRegAndBranch(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus CompareRegSimmAndBranch(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus CompareRegUimmAndBranch(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus CompareRegZeroAndBranch(const CMD cmd);

    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus CompareAndTrap(const CMD cmd);

    // load/store templates
    template<class T, class CMD> ALWAYS_INLINE CStatus read_base(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus write_base(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus read_ip_relative(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus write_ip_relative(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus read_indexed(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus write_indexed(const CMD cmd);

    // load/store with post-update templates
    template<class T, class CMD> ALWAYS_INLINE CStatus LoadModifyImmediateAfter(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus StoreModifyImmediateAfter(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus LoadModifyImmediateBefore(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus StoreModifyImmediateBefore(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus LoadModifyIndexedAfter(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus StoreModifyIndexedAfter(const CMD cmd);

    // atomic templates
    template<class T, class CMD> ALWAYS_INLINE CStatus read_atomic(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus write_atomic(const CMD cmd);
    template<class T, class OP, class CMD> ALWAYS_INLINE CStatus fetch_and_op_atomic(const CMD cmd);
    template<class T, class CMD> ALWAYS_INLINE CStatus compare_and_swap(const CMD cmd);

    // fpu/vector/simd templates
    template<class T, class CMD> CStatus SimdMergeLow(const CMD cmd);
    template<class T, class CMD> CStatus SimdMergeHigh(const CMD cmd);

    template<class T, class WT, class CMD> CStatus SimdUnpackLow(const CMD cmd);
    template<class T, class WT, class CMD> CStatus SimdUnpackHigh(const CMD cmd);
    template<class T, class WT, class OP, class CMD> CStatus VectorPack(const CMD cmd);

    template<class T, class OP, class CMD> CStatus VectorBinaryRegReg(const CMD cmd);
    template<class T, class OP, class CMD> CStatus VectorBinaryRegShamt(const CMD cmd);
    template<class T, class OP, class CMD> CStatus VectorCompareRegReg(const CMD cmd);

    template<class D, class T, class OP, class CMD> CStatus ScalarOp2(const CMD cmd);
    template<class D, class T, class OP, class CMD> CStatus ScalarOp2RoundingMode(const CMD cmd);
    template<class D, class T, class OP, class CMD> CStatus ScalarOp3(const CMD cmd);
    template<class D, class T, class OP, class PARAM, class CMD> CStatus ScalarOp3(const CMD cmd, PARAM param);
    template<class D, class T, class OP, class CMD> CStatus ScalarOp4(const CMD cmd);

    template<class D, class T, class OP, class CMD> CStatus VectorOp2(const CMD cmd);
    template<class D, class T, class OP, class CMD> CStatus VectorOp2RoundingMode(const CMD cmd);
    template<class D, class T, class OP, class CMD> CStatus VectorOp3(const CMD cmd);
    template<class D, class T, class OP, class PARAM, class CMD> CStatus VectorOp3(const CMD cmd, PARAM param);
    template<class D, class T, class OP, class CMD> CStatus VectorOp4(const CMD cmd);

    template<class CMD> CStatus PackedF32MulAddAlternate(const CMD cmd, fpu::muladd_negate_t even_sig, fpu::muladd_negate_t odd_sig);
    template<class CMD> CStatus PackedF64MulAddAlternate(const CMD cmd, fpu::muladd_negate_t even_sig, fpu::muladd_negate_t odd_sig);

// prototypes for all instructions
#define DEFINE_INSN_IMPLEMENTATION_PROTO(name) \
    template<class CMD> ALWAYS_INLINE CStatus insn_impl_##name(const CMD cmd); \
    template<class CMD> static CStatus insn_##name(Core & core, const CMD cmd) { return core.insn_impl_##name(cmd); }

    DECLARE_PRIMARY_OPCODES(DEFINE_INSN_IMPLEMENTATION_PROTO)
    DECLARE_MISC_EXTENDED_OPCODES(DEFINE_INSN_IMPLEMENTATION_PROTO)
    DECLARE_FUSED_EXTENDED_OPCODES(DEFINE_INSN_IMPLEMENTATION_PROTO)
    DECLARE_RAOPX_EXTENDED_OPCODES(DEFINE_INSN_IMPLEMENTATION_PROTO)
    DECLARE_NULLIFY_EXTENDED_OPCODES(DEFINE_INSN_IMPLEMENTATION_PROTO)

    // for tpa instruction
    CStatus mmu_translate(uint64_t virt_address, phys_address_t& phys_address);
    CStatus dump_virt_map_sub_dir(const DumpFormatter& out, const phys_address_t pta,
        uint64_t base, uint64_t & used, uint64_t & extra_used, int level) const;

    template<class TranslationHelper> CStatus
    translate_address(CCoreTranslationCache & tc,
        const uint64_t virt_address, uint64_t requested_rights,
        uint64_t unexpected_rights, phys_address_t& phys_address);

    CStatus InsertPage(uint64_t virt_address, phys_address_t phys_address, uint64_t attributes);

private:
    template<class Archive>
    void
    serialize(Archive & ar, const unsigned int version);

private:
    CRegisterStackControlRegister rsc;     // register stack control
    uint64_t                      ip;      // instruction pointer
    fpu::CSoftFPU                 fpu;     // floating-point control

    std::array<CGeneralRegister, num_static_regs + phys_stacked_regs>  gpr;   // general registers

    uint64_t                      iip;     // interrupt instruction pointer
    uint64_t                      iipa;    // interrupt instruction previous address (with ri (restart instruction) field in 2 lowest bits)
    uint64_t                      rsp;     // register stack pointer (bottom of dirty regs in memory)
    uint64_t                      peb;     // process env block
    uint64_t                      teb;     // thread env block
    uint64_t                      itc;     // interval time counter
    uint64_t                      itm;     // interval time match register
    CProcessorStatusRegister      psr;     // processor status register
    CProcessorStatusRegister      ipsr;    // processor status register, interruption copy
    CInterruptionCauseRegister    cause;   // interruption cause register
    uint64_t                      kip;     // syscall, kernel instruction pointer
    uint64_t                      krsp;    // syscall, kernel register stack pointer
    uint64_t                      ksp;     // syscall, kernel stack pointer
    uint64_t                      iva;     // interruption vector address
    uint64_t                      ifa;     // interruption: faulting address
    CBundle                       iib;     // interruption instruction bundle
    pic_t                         pic;     // local programmable interrupt controller
    TranslationUnit               mmu;     // memory management unit
    CRandomUnit                   random;  // random generator unit
    CpuIdentificationStore        cpuid;   // cpuid read-only store

    CCoreTranslationCache         m_DataTranslationCache;
    CCoreTranslationCache         m_CodeTranslationCache;

    std::array<phys_address_t, ICACHE_SIZE / ICACHE_LINESIZE>  m_instruction_cache_tags;
    alignas(16) std::array<InstructionCacheLine<ICACHE_LINESIZE>,
                           ICACHE_SIZE / ICACHE_LINESIZE>    m_instruction_cache_data;

    // exception instruction pointer (eip), exception context (eca)
    SoftExceptionState            exception_state;

    std::array<CInstructionBreakpointRegister, 4>   ibr;
    std::array<CDataBreakpointRegister, 4>          dbr;

    CPerformanceMonitor           performance_monitor;

    double                                m_RunTime;
    std::map<uint64_t, uint32_t>          profile;
    unsigned                              config_flags;
    bool                                  m_Stopped;

    uint64_t                              debug_ip;
    unsigned                              debug_ri;
    CoreConfigSpace                       config;
};

#define DEFINE_INSN_IMPLEMENTATION(name) \
    template<class CMD> ALWAYS_INLINE CStatus Core::insn_impl_##name(const CMD cmd)

template<class Archive>
void
Core::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    LOG_TRACE(LOG_SERIALIZATION) << "enter";

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
    ar & BOOST_SERIALIZATION_NVP(profile);
    ar & BOOST_SERIALIZATION_NVP(m_Stopped);
    ar & BOOST_SERIALIZATION_NVP(debug_ip);
    ar & BOOST_SERIALIZATION_NVP(debug_ri);
    ar & BOOST_SERIALIZATION_NVP(m_DataTranslationCache);
    ar & BOOST_SERIALIZATION_NVP(m_CodeTranslationCache);
    ar & BOOST_SERIALIZATION_NVP(m_instruction_cache_tags);
    ar & BOOST_SERIALIZATION_NVP(m_instruction_cache_data);
}

ALWAYS_INLINE
CGeneralRegister &
Core::wget(unsigned phys_regnum)
{
    CGeneralRegister & reg = gpr[ phys_regnum ];
    // LOG_TRACE(LOG_REGISTER_DATA) << "before=" << phys_regnum << ":" << reg;
    return reg;
}

ALWAYS_INLINE
const CGeneralRegister &
Core::rget(unsigned phys_regnum) const
{
    const CGeneralRegister & reg = gpr[ phys_regnum ];
    // LOG_TRACE(LOG_REGISTER_DATA) << "read=" << phys_regnum << ":" << reg;
    return reg;
}

#define STRUCT_MEMBER_SIZE(struct_, member_)  sizeof(((struct_ *)nullptr) -> member_)

/***************************************************************************
* register windows/rotation macros
* used inside core class member functions only
***************************************************************************/

#define ROTATE(PREFIX, REGSOURCE)                                     \
    const unsigned PREFIX##regnum = REGSOURCE;                        \
    unsigned PREFIX##result;                                          \
    if (likely(is_rotate_reg(PREFIX##regnum))) {                      \
        if (unlikely(rsc.IsOutOfFrame(PREFIX##regnum))) {             \
            LOG_ERROR(LOG_REGISTER_ROTATION | LOG_EXCEPTION)          \
                << "rotate reg=" << PREFIX##regnum                    \
                << " SizeOfFrame=" << rsc.GetSizeOfFrame();           \
                                                                      \
            return CStatus(CStatus::register_window_protection);      \
        }                                                             \
        PREFIX##result = rsc.rotate(PREFIX##regnum);                  \
        LOG_TRACE(LOG_REGISTER_ROTATION)                              \
            << "rotate reg=" << PREFIX##regnum                        \
            << " physical=" << PREFIX##result;                        \
    } else {                                                          \
        PREFIX##result = rsc.RegStaticToPhys(PREFIX##regnum);         \
        LOG_TRACE(LOG_REGISTER_ROTATION)                              \
            << "static reg=" << PREFIX##regnum                        \
            << " physical=" << PREFIX##result;                        \
    }                                                                 \
                                                                      \
    LOG_TRACE(LOG_REGISTER_DATA)                                      \
        << "reg:" << PREFIX##regnum                                   \
        << ":" << PREFIX##result                                      \
        << ":" << gpr[ PREFIX##result ];                              \


#define GET_RRA  ROTATE(ria_, cmd.ra())  const CGeneralRegister & a = rget( ria_result )
#define GET_RRB  ROTATE(rib_, cmd.rb())  const CGeneralRegister & b = rget( rib_result )
#define GET_RRC  ROTATE(ric_, cmd.rc())  const CGeneralRegister & c = rget( ric_result )
#define GET_RRD  ROTATE(rid_, cmd.rd())  const CGeneralRegister & d = rget( rid_result )
#define GET_WRA  ROTATE(ria_, cmd.ra())        CGeneralRegister & a = wget( ria_result )
#define GET_WRB  ROTATE(rib_, cmd.rb())        CGeneralRegister & b = wget( rib_result )

#define XR(REGNUM) rget( rsc.RegStaticToPhys(REGNUM) )
#define XW(REGNUM) wget( rsc.RegStaticToPhys(REGNUM) )

#define CHECK_KERNEL_MODE()                                                \
    if (unlikely(!psr.IsKernelMode())) {                                   \
        LOG_ERROR(LOG_EXCEPTION) << "privileged instruction: " << cmd;     \
        return CStatus(CStatus::general_protection);                       \
    }

#define CHECK_STATUS_ADDRESS(address, status)                              \
    do {                                                                   \
        CStatus status__ = (status);                                       \
        if (unlikely(!status__.continuable())) {                           \
            SetFaultingAddress(address);                                   \
            return status__;                                               \
        }                                                                  \
    } while (0)

} // namespace postrisc
