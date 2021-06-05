#pragma once

#include "fpu/float.hpp"
#include "instruction_fields.hpp"
#include "opcode.hpp"
#include "fpu/float.hpp"

namespace postrisc {

/***************************************************************************
* sign or zero field-extraction functions
***************************************************************************/

// constexpr inline uint64_t get_ubf(uint64_t cmd, unsigned int disp, unsigned int size, disp, size) const { return (src << (64-disp-size)) >> (64-size); }

constexpr ALWAYS_INLINE uint64_t get_ubitfield(uint64_t src, unsigned int disp, unsigned int size)
{
    return (src >> disp) & util::makemask(size);
}

constexpr ALWAYS_INLINE int64_t get_sbitfield(int64_t src, unsigned int disp, unsigned int size)
{
    return (src << (64-disp-size)) >> (64-size);
}

/***************************************************************************
* single-slot instruction
***************************************************************************/
class CInsn1 {
public:
    constexpr explicit CInsn1(uint64_t c) : cmd(c) {}

    uint64_t GetRaw(void) const { return cmd; }

    ALWAYS_INLINE unsigned GetOpcode(void) const {
        return static_cast<unsigned>(get_u(disp_opcode, bits_per_opcode));
    }
    void SetOpcode(unsigned opcode) {
        set_u(opcode, disp_opcode, bits_per_opcode);
    }

    ALWAYS_INLINE unsigned int ra(void) const { return GetRegister(disp_reg_a); }
    ALWAYS_INLINE unsigned int rb(void) const { return GetRegister(disp_reg_b); }
    ALWAYS_INLINE unsigned int rc(void) const { return GetRegister(disp_reg_c); }
    ALWAYS_INLINE unsigned int rd(void) const { return GetRegister(disp_reg_d); }

    // special register
    ALWAYS_INLINE unsigned int rs(void) const { return GetRegister(disp_reg_b); }

    void set_ra(unsigned int regnum) { SetRegister(regnum, disp_reg_a); }
    void set_rb(unsigned int regnum) { SetRegister(regnum, disp_reg_b); }
    void set_rc(unsigned int regnum) { SetRegister(regnum, disp_reg_c); }
    void set_rd(unsigned int regnum) { SetRegister(regnum, disp_reg_d); }

    // special register
    void set_rs(unsigned int regnum) { SetRegister(regnum, disp_reg_b); }

    ALWAYS_INLINE unsigned int get_shamt_b(void) const { return GetShiftAmount(disp_reg_b); }
    ALWAYS_INLINE unsigned int get_shamt_c(void) const { return GetShiftAmount(disp_reg_c); }
    ALWAYS_INLINE unsigned int get_shamt_d(void) const { return GetShiftAmount(disp_reg_d); }
    ALWAYS_INLINE unsigned int get_shamt_e(void) const { return GetShiftAmount(disp_reg_e); }

    void set_shamt_b(unsigned int shamt) { return SetShiftAmount(shamt, disp_reg_b); }
    void set_shamt_c(unsigned int shamt) { return SetShiftAmount(shamt, disp_reg_c); }
    void set_shamt_d(unsigned int shamt) { return SetShiftAmount(shamt, disp_reg_d); }
    void set_shamt_e(unsigned int shamt) { return SetShiftAmount(shamt, disp_reg_e); }

    ALWAYS_INLINE unsigned int get_vshamt_b(void) const { return get_vshamt(disp_reg_b); }
    ALWAYS_INLINE unsigned int get_vshamt_c(void) const { return get_vshamt(disp_reg_c); }
    ALWAYS_INLINE unsigned int get_vshamt_d(void) const { return get_vshamt(disp_reg_d); }
    ALWAYS_INLINE unsigned int get_vshamt_e(void) const { return get_vshamt(disp_reg_e); }

    void set_vshamt_b(unsigned int vshamt) { return set_vshamt(vshamt, disp_reg_b); }
    void set_vshamt_c(unsigned int vshamt) { return set_vshamt(vshamt, disp_reg_c); }
    void set_vshamt_d(unsigned int vshamt) { return set_vshamt(vshamt, disp_reg_d); }
    void set_vshamt_e(unsigned int vshamt) { return set_vshamt(vshamt, disp_reg_e); }

    ALWAYS_INLINE uint64_t get_nullify_yes(void) const { return get_u(disp_nullify_dist_yes, bits_per_nullify_dist); }
    ALWAYS_INLINE uint64_t get_nullify_not(void) const { return get_u(disp_nullify_dist_no , bits_per_nullify_dist); }

    void set_nullify_yes(uint64_t mask) { return set_u(mask, disp_nullify_dist_yes, bits_per_nullify_dist); }
    void set_nullify_not(uint64_t mask) { return set_u(mask, disp_nullify_dist_no, bits_per_nullify_dist); }

    ALWAYS_INLINE int64_t GetIndexedDisplacement(void) const { return get_s(disp_indexed_disp, bits_per_indexed_disp); }
    void SetIndexedDisplacement(int64_t disp) { return set_s(disp, disp_indexed_disp, bits_per_indexed_disp); }

    ALWAYS_INLINE unsigned GetIndexedScale(void) const {
        return static_cast<unsigned>(get_u(disp_indexed_scale, bits_per_indexed_scale));
    }
    void SetIndexedScale(unsigned scale) { return set_u(scale, disp_indexed_scale, bits_per_indexed_scale); }

    ALWAYS_INLINE EMemoryOrdering GetMemoryOrdering(void) const {
        return static_cast<EMemoryOrdering>(get_u(disp_memory_ordering, bits_per_memory_ordering));
    }
    void SetMemoryOrdering(EMemoryOrdering ordering) { return set_u(ordering, disp_memory_ordering, bits_per_memory_ordering); }
   
    ALWAYS_INLINE int64_t GetLoopStep(void) const {
        return get_s(disp_loop_step, bits_per_loop_step);
    }
    void SetLoopStep(int64_t step) { return set_s(step, disp_loop_step, bits_per_loop_step); }

    ALWAYS_INLINE int64_t GetUpdateStride(void) const { return get_s(disp_update_stride, bits_per_update_stride); }
    void SetUpdateStride(int64_t stride) { return set_s(stride, disp_update_stride, bits_per_update_stride); }

    ALWAYS_INLINE uint64_t GetUintStride(void) const { return get_u(disp_update_stride, bits_per_update_stride); }
    void SetUintStride(uint64_t stride) { return set_u(stride, disp_update_stride, bits_per_update_stride); }

    ALWAYS_INLINE unsigned GetRoundingMode(void) const {
        return static_cast<unsigned>(get_u(disp_rounding_mode, bits_per_rounding_mode));
    }
    void SetRoundingMode(unsigned rounding_mode) {
        return set_u(rounding_mode, disp_rounding_mode, bits_per_rounding_mode);
    }

    ALWAYS_INLINE fpu::compare_mode_t GetFpCompareMode(void) const {
        return static_cast<fpu::compare_mode_t>(get_u(disp_fp_compare_mode, bits_per_fp_compare_mode));
    }
    void SetFpCompareMode(unsigned mode) {
        return set_u(mode, disp_fp_compare_mode, bits_per_fp_compare_mode);
    }

    ALWAYS_INLINE int64_t get_bin_simm(void) const { return get_s(disp_binimm, bits_per_binimm); }
    void set_bin_simm(int64_t imm) { return set_s(imm, disp_binimm, bits_per_binimm); }

    ALWAYS_INLINE uint64_t get_bin_uimm(void) const { return get_u(disp_binimm, bits_per_binimm); }
    void set_bin_uimm(uint64_t imm) { return set_u(imm, disp_binimm, bits_per_binimm); }

    template<class T> T get_bin_imm(void) const;
    template<int64_t> int64_t get_bin_imm(void) const { return get_s(disp_binimm, bits_per_binimm); }
    template<uint64_t> uint64_t get_bin_imm(void) const { return get_u(disp_binimm, bits_per_binimm); }

    ALWAYS_INLINE int64_t get_brc_simm(void) const { return get_s(disp_brci_imm, bits_per_brci_imm); }
    uint64_t get_brc_uimm(void) const { return get_u(disp_brci_imm, bits_per_brci_imm); }

    void set_brc_simm(int64_t imm) { return set_s(imm, disp_brci_imm, bits_per_brci_imm); }
    void set_brc_uimm(uint64_t imm) { return set_u(imm, disp_brci_imm, bits_per_brci_imm); }

    ALWAYS_INLINE int64_t get_ldi(void) const { return get_s(disp_branch, bits_per_branch); }
    void set_ldi(int64_t imm) { return set_s(imm, disp_branch, bits_per_branch); }

    ALWAYS_INLINE int64_t GetJumpDisplacement(void) const { return get_s(disp_branch, bits_per_branch); }
    void SetJumpDisplacement(int64_t branch) { return set_s(branch, disp_branch, bits_per_branch); }

    ALWAYS_INLINE uint64_t ComputeJumpRelative(uint64_t ip) const { return ip + bytes_per_bundle * GetJumpDisplacement(); }

    ALWAYS_INLINE uint64_t GetForwardBranch(void) const { return get_u(disp_branch, bits_per_branch); }
    void SetForwardBranch(uint64_t branch) { return set_u(branch, disp_branch, bits_per_branch); }
    ALWAYS_INLINE uint64_t ComputeForwardRelative(uint64_t ip) const { return ip + GetForwardBranch(); }

    ALWAYS_INLINE int64_t GetBranchDisplacement(void) const { return get_s(disp_brc_branch, bits_per_brc_branch); }
    void SetBranchDisplacement(int64_t branch) { return set_s(branch, disp_brc_branch, bits_per_brc_branch); }

    ALWAYS_INLINE int64_t GetShortBranchDisplacement(void) const { return get_s(disp_short_branch, bits_per_short_branch); }
    void SetShortBranchDisplacement(int64_t branch) { return set_s(branch, disp_short_branch, bits_per_short_branch); }

    ALWAYS_INLINE uint64_t ComputeBranchRelative(uint64_t ip) const { return ip + bytes_per_bundle * GetBranchDisplacement(); }
    ALWAYS_INLINE uint64_t ComputeShortBranchRelative(uint64_t ip) const { return ip + bytes_per_bundle * GetShortBranchDisplacement(); }

    ALWAYS_INLINE uint64_t GetLoadAddressNear(uint64_t base) const { return base + (get_bin_simm() << bits_per_ldan_shift); }

    ALWAYS_INLINE unsigned int GetMiscOpx(void) const {
        return static_cast<unsigned>(get_u(disp_opx, bits_per_misc_opx));
    }
    ALWAYS_INLINE unsigned int GetFusedOpx(void) const {
        return static_cast<unsigned>(get_u(disp_opx, bits_per_fused_opx));
    }
    ALWAYS_INLINE unsigned int GetRaOpx(void) const {
        return GetRegister(disp_reg_a);
    }
    ALWAYS_INLINE unsigned int GetNullifyOpx(void) const {
        return static_cast<unsigned>(get_u(disp_opx, bits_per_nullify_opx));
    }

    friend std::ostream& operator << (std::ostream& out, const CInsn1 cmd);

protected:
    ALWAYS_INLINE int64_t get_s(EInstructionOffset disp, EInstructionSize size) const
        { return get_sbitfield(cmd, static_cast<unsigned>(disp), static_cast<unsigned>(size)); }
    ALWAYS_INLINE uint64_t get_u(EInstructionOffset disp, EInstructionSize size) const
        { return get_ubitfield(cmd, static_cast<unsigned>(disp), static_cast<unsigned>(size)); }

    void set_s(int64_t value, EInstructionOffset disp, EInstructionSize size)
    {
        value &= util::makemask(size);
        cmd |= value << disp;
    }

    void set_u(uint64_t value, EInstructionOffset disp, EInstructionSize size)
    {
        value &= util::makemask(size);
        cmd |= value << disp;
    }

    // get register number in various positions
    ALWAYS_INLINE unsigned int GetRegister(const EInstructionOffset disp) const {
        return static_cast<unsigned>(get_u(disp, bits_per_register));
    }
    void SetRegister(unsigned regnum, const EInstructionOffset disp) {
        set_u(regnum, disp, bits_per_register);
    }

    // get shift amounts in various positions
    ALWAYS_INLINE unsigned int GetShiftAmount(const EInstructionOffset disp) const {
        return static_cast<unsigned>(get_u(disp, bits_per_shamt));
    }
    void SetShiftAmount(unsigned int shamt, const EInstructionOffset disp) {
        set_u(shamt, disp, bits_per_shamt);
    }

    // get vector shift amounts in various positions
    ALWAYS_INLINE unsigned int get_vshamt(const EInstructionOffset disp) const {
        return static_cast<unsigned>(get_u(disp, bits_per_vshamt));
    }
    void set_vshamt(unsigned int shamt, const EInstructionOffset disp) {
        return set_u(shamt, disp, bits_per_vshamt);
    }

public:
    static const int s_num_slots = 1;

protected:
    uint64_t  cmd;
};

/***************************************************************************
* double-slot instruction
***************************************************************************/
class CInsn2 : public CInsn1 {
public:
    constexpr explicit CInsn2(uint64_t c, uint64_t e)
        : CInsn1(c), ext(e) {}

    uint64_t GetRawExtension(void) const {
        return ext;
    }

    int64_t get_bin_simm(void) const {
        return get_sx(disp_binimm, bits_per_binimm,
                      disp_binimm_ext, bits_per_binimm_ext);
    }

    void set_bin_simm(int64_t imm) {
        return set_sx(imm, disp_binimm, bits_per_binimm,
                           disp_binimm_ext, bits_per_binimm_ext);
    }

    uint64_t get_bin_uimm(void) const {
        return get_ux(disp_binimm, bits_per_binimm,
                      disp_binimm_ext, bits_per_binimm_ext);
    }

    void set_bin_uimm(uint64_t imm) {
        return set_ux(imm, disp_binimm, bits_per_binimm,
                           disp_binimm_ext, bits_per_binimm_ext);
    }

    int64_t get_ldi(void) const {
        return get_sx(disp_branch, bits_per_branch,
                      disp_ldi_ext, bits_per_ldi_ext);
    }

    void set_ldi(int64_t imm) {
        return set_sx(imm, disp_branch, bits_per_branch,
                           disp_ldi_ext, bits_per_ldi_ext);
    }

    int64_t GetJumpDisplacement(void) const {
        return get_sx(disp_branch, bits_per_branch,
                      disp_branch_ext, bits_per_branch_ext);
    }

    void SetJumpDisplacement(int64_t branch) {
        return set_sx(branch, disp_branch, bits_per_branch,
                              disp_branch_ext, bits_per_branch_ext);
    }

    int64_t GetBranchDisplacement(void) const {
        return get_sx(disp_brc_branch, bits_per_brc_branch,
                      disp_brc_branch_ext, bits_per_brc_branch_ext);
    }

    void SetBranchDisplacement(int64_t branch) {
        return set_sx(branch, disp_brc_branch, bits_per_brc_branch,
                              disp_brc_branch_ext, bits_per_brc_branch_ext);
    }

    int64_t GetShortBranchDisplacement(void) const {
        return get_sx(disp_short_branch, bits_per_short_branch,
                      disp_short_branch_ext, bits_per_short_branch_ext);
    }

    void SetShortBranchDisplacement(int64_t branch) {
        return set_sx(branch, disp_short_branch, bits_per_short_branch,
                              disp_short_branch_ext, bits_per_short_branch_ext);
    }

    uint64_t ComputeJumpRelative(uint64_t ip) const {
        return ip + bytes_per_bundle * GetJumpDisplacement();
    }

    uint64_t GetForwardBranch(void) const {
        return get_ux(disp_branch, bits_per_branch, disp_ldi_ext, bits_per_ldi_ext);
    }

    void SetForwardBranch(uint64_t branch) {
        return set_ux(branch, disp_branch, bits_per_branch,
                              disp_ldi_ext, bits_per_ldi_ext);
    }

    uint64_t ComputeForwardRelative(uint64_t ip) const {
        return ip + GetForwardBranch();
    }

    int64_t get_brc_simm(void) const {
        return get_sx(disp_brci_imm, bits_per_brci_imm,
                      disp_brci_imm_ext, bits_per_brci_imm_ext); }

    uint64_t get_brc_uimm(void) const {
        return get_ux(disp_brci_imm, bits_per_brci_imm,
                      disp_brci_imm_ext, bits_per_brci_imm_ext); }

    void set_brc_simm(int64_t imm) {
        return set_sx(imm, disp_brci_imm, bits_per_brci_imm,
                           disp_brci_imm_ext, bits_per_brci_imm_ext); }

    void set_brc_uimm(uint64_t imm) {
        return set_ux(imm, disp_brci_imm, bits_per_brci_imm,
                           disp_brci_imm_ext, bits_per_brci_imm_ext); }

    int64_t GetIndexedDisplacement(void) const {
        return get_sx(disp_indexed_disp, bits_per_indexed_disp,
                      disp_indexed_disp_ext, bits_per_indexed_disp_ext);
    }

    void SetIndexedDisplacement(int64_t disp) {
        return set_sx(disp, disp_indexed_disp, bits_per_indexed_disp,
                            disp_indexed_disp_ext, bits_per_indexed_disp_ext);
    }

    uint64_t ComputeBranchRelative(uint64_t ip) const {
        return ip + bytes_per_bundle * GetBranchDisplacement();
    }

    uint64_t ComputeShortBranchRelative(uint64_t ip) const {
        return ip + bytes_per_bundle * GetShortBranchDisplacement();
    }

    uint64_t GetLoadAddressNear(uint64_t base) const {
        return base + (get_bin_simm() << bits_per_ldan_shift);
    }

    friend std::ostream& operator << (std::ostream& out, const CInsn2 cmd);

private:
    int64_t get_sx(EInstructionExtensionOffset disp, EInstructionExtensionSize size) const
    {
        return get_sbitfield(ext, (unsigned)disp, (unsigned)size);
    }

    uint64_t get_ux(EInstructionExtensionOffset disp, EInstructionExtensionSize size) const
    {
        return get_ubitfield(ext, (unsigned)disp, (unsigned)size);
    }

    int64_t get_sx(EInstructionOffset disp, EInstructionSize size,
               EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size) const
    {
        return get_u(disp, size) | (get_sx(ext_disp, ext_size) << size);
    }

    uint64_t get_ux(EInstructionOffset disp, EInstructionSize size,
               EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size) const
    {
        return get_u(disp, size) | (get_ux(ext_disp, ext_size) << size);
    }

    void set_sx(int64_t value, EInstructionExtensionOffset disp, EInstructionExtensionSize size)
    {
        value &= util::makemask(size);
        ext |= value << disp;
    }

    void set_ux(uint64_t value, EInstructionExtensionOffset disp, EInstructionExtensionSize size)
    {
        value &= util::makemask(size);
        ext |= value << disp;
    }

    void set_ux(uint64_t value, EInstructionOffset disp, EInstructionSize size,
                EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size)
    {
        set_u(value, disp, size);
        set_ux(value >> size, ext_disp, ext_size);
    }

    void set_sx(int64_t value, EInstructionOffset disp, EInstructionSize size,
                EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size)
    {
        set_s(value, disp, size);
        set_sx(value >> size, ext_disp, ext_size);
    }

public:
    static const int s_num_slots = 2;

protected:
    uint64_t  ext;
};


/***************************************************************************
* bundle of instructions
***************************************************************************/
enum class EBundleTemplate {
    bundle_sss = 0,
    bundle_sll = 1,
    bundle_lls = 2,
    bundle_lll = 3 //reserved
};

class CBundle {
public:
    explicit CBundle() : lo(0), hi(0) {}
    explicit CBundle(const uint64_t slot[slots_per_bundle], EBundleTemplate tmplt);

    uint64_t get_lo(void) const { return lo; }
    uint64_t get_hi(void) const { return hi; }

    EBundleTemplate GetTemplate(void) const {
        return static_cast<EBundleTemplate>(lo & util::makemask(bits_per_bundle_template));
    }

    const CInsn1 s0(void) const { return CInsn1(GetSlotA()); }
    const CInsn1 s1(void) const { return CInsn1(GetSlotB()); }
    const CInsn1 s2(void) const { return CInsn1(GetSlotC()); }
    const CInsn2 l0(void) const { return CInsn2(GetSlotA(), GetSlotB()); }
    const CInsn2 l1(void) const { return CInsn2(GetSlotB(), GetSlotC()); }

    fpu::u128 v128(void) const
    {
        return fpu::u128(hi, lo);
    }

    friend std::ostream& operator << (std::ostream & out, const CBundle & bundle);

    template<class Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(lo);
        ar & BOOST_SERIALIZATION_NVP(hi);
    }

private:
    uint64_t GetSlotA(void) const {
        return  (lo >> bits_per_bundle_template) & util::makemask(bits_per_slot);
    }
    uint64_t GetSlotB(void) const {
        return ((lo >> (bits_per_bundle_template + bits_per_slot)) |
                (hi << (64 - bits_per_slot - bits_per_bundle_template)))
               & util::makemask(bits_per_slot);
    }
    uint64_t GetSlotC(void) const {
        return  (hi >> (64 - bits_per_slot)) & util::makemask(bits_per_slot);
    }

private:
    uint64_t   lo, hi; // assume little endian order (architecture dependent)
};

} // namespace postrisc
