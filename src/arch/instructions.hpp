#pragma once

#include "util/common.hpp"
#include "instruction_fields.hpp"
#include "opcode.hpp"

namespace postrisc {

/***************************************************************************
* sign or zero field-extraction functions
***************************************************************************/

// constexpr inline u64 get_ubf(u64 cmd, unsigned int disp, unsigned int size, disp, size) const { return (src << (64-disp-size)) >> (64-size); }

constexpr ALWAYS_INLINE u64 get_ubitfield(u64 src, unsigned int disp, unsigned int size)
{
    return (src >> disp) & util::makemask(size);
}

constexpr ALWAYS_INLINE i64 get_sbitfield(i64 src, unsigned int disp, unsigned int size)
{
    return (src << (64-disp-size)) >> (64-size);
}

/***************************************************************************
* single-slot instruction
***************************************************************************/
class CInsn1 {
public:
    constexpr explicit CInsn1(u64 c) : cmd(c) {}

    u64 GetRaw(void) const { return cmd; }

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
    ALWAYS_INLINE unsigned int rs(void) const { return GetRegister(disp_reg_c); }

    void set_ra(unsigned int regnum) { SetRegister(regnum, disp_reg_a); }
    void set_rb(unsigned int regnum) { SetRegister(regnum, disp_reg_b); }
    void set_rc(unsigned int regnum) { SetRegister(regnum, disp_reg_c); }
    void set_rd(unsigned int regnum) { SetRegister(regnum, disp_reg_d); }

    // special register
    void set_rs(unsigned int regnum) { SetRegister(regnum, disp_reg_c); }

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

    // nullification distances
    ALWAYS_INLINE u64 get_nullify_yes(void) const {
        return get_u(disp_nullify_dist_yes, bits_per_nullify_dist);
    }
    ALWAYS_INLINE u64 get_nullify_not(void) const {
        return get_u(disp_nullify_dist_no , bits_per_nullify_dist);
    }

    void set_nullify_yes(u64 mask) {
        return set_u(mask, disp_nullify_dist_yes, bits_per_nullify_dist);
    }
    void set_nullify_not(u64 mask) {
        return set_u(mask, disp_nullify_dist_no, bits_per_nullify_dist);
    }

    ALWAYS_INLINE i64 GetIndexedDisplacement(void) const {
        return get_s(disp_indexed_disp, bits_per_indexed_disp);
    }
    void SetIndexedDisplacement(i64 disp) {
        return set_s(disp, disp_indexed_disp, bits_per_indexed_disp);
    }

    ALWAYS_INLINE unsigned GetIndexedScale(void) const {
        return static_cast<unsigned>(get_u(disp_indexed_scale, bits_per_indexed_scale));
    }
    void SetIndexedScale(unsigned scale) {
        return set_u(scale, disp_indexed_scale, bits_per_indexed_scale);
    }

    ALWAYS_INLINE EScaleMode GetIndexedScaleMode(void) const {
        return static_cast<EScaleMode>(get_u(disp_indexed_scale_mode, bits_per_indexed_scale_mode));
    }
    void SetIndexedScaleMode(EScaleMode scale) {
        return set_u(scale, disp_indexed_scale_mode, bits_per_indexed_scale_mode);
    }

    ALWAYS_INLINE i64 GetUpdateStride(void) const {
        return get_s(disp_update_stride, bits_per_update_stride);
    }
    void SetUpdateStride(i64 stride) {
        return set_s(stride, disp_update_stride, bits_per_update_stride);
    }

    ALWAYS_INLINE unsigned GetRoundingMode(void) const {
        return static_cast<unsigned>(get_u(disp_rounding_mode, bits_per_rounding_mode));
    }
    void SetRoundingMode(unsigned rounding_mode) {
        return set_u(rounding_mode, disp_rounding_mode, bits_per_rounding_mode);
    }

    ALWAYS_INLINE i64 get_bin_simm(void) const {
        return get_s(disp_binimm, bits_per_binimm);
    }
    void set_bin_simm(i64 imm) {
        return set_s(imm, disp_binimm, bits_per_binimm);
    }

    ALWAYS_INLINE u64 get_bin_uimm(void) const {
        return get_u(disp_binimm, bits_per_binimm);
    }
    void set_bin_uimm(u64 imm) {
        return set_u(imm, disp_binimm, bits_per_binimm);
    }

    template<class T> T get_bin_imm(void) const;
    template<i64> i64 get_bin_imm(void) const { return get_s(disp_binimm, bits_per_binimm); }
    template<u64> u64 get_bin_imm(void) const { return get_u(disp_binimm, bits_per_binimm); }

    ALWAYS_INLINE i64 get_brc_simm(void) const {
        return get_s(disp_brci_imm, bits_per_brci_imm);
    }

    void set_brc_simm(i64 imm) { return set_s(imm, disp_brci_imm, bits_per_brci_imm); }

    ALWAYS_INLINE u64 get_brc_uimm(void) const {
        return get_u(disp_brci_imm, bits_per_brci_imm);
    }

    void set_brc_uimm(u64 imm) { return set_u(imm, disp_brci_imm, bits_per_brci_imm); }

    ALWAYS_INLINE i64 get_ldi(void) const {
        return get_s(disp_call, bits_per_call);
    }
    void set_ldi(i64 imm) {
        return set_s(imm, disp_call, bits_per_call);
    }

    ALWAYS_INLINE i64 GetJumpDisplacement(void) const {
        return get_s(disp_call, bits_per_call);
    }
    void SetJumpDisplacement(i64 branch) {
        return set_s(branch, disp_call, bits_per_call);
    }

    // alloc/allocsp framesize
    ALWAYS_INLINE unsigned int GetFrameSize(void) const {
        return static_cast<unsigned>(get_u(disp_framesize, bits_per_framesize));
    }
    void SetFrameSize(unsigned mode) {
        return set_u(mode, disp_framesize, bits_per_framesize);
    }

    // various computation helpers
    ALWAYS_INLINE u64 ComputeJumpRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetJumpDisplacement();
    }

    ALWAYS_INLINE u64 GetForwardBranch(void) const {
        return get_u(disp_call, bits_per_call);
    }
    void SetForwardBranch(u64 branch) {
        return set_u(branch, disp_call, bits_per_call);
    }

    ALWAYS_INLINE u64 ComputeForwardRelative(u64 ip) const {
        return ip + forward_offset_alignment * GetForwardBranch();
    }

    ALWAYS_INLINE i64 GetBranchDisplacement(void) const {
        return get_s(disp_branch, bits_per_branch);
    }
    void SetBranchDisplacement(i64 branch) {
        return set_s(branch, disp_branch, bits_per_branch);
    }

    ALWAYS_INLINE u64 ComputeBranchRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetBranchDisplacement();
    }

    // opcode/opx, precomputed, never set
    ALWAYS_INLINE unsigned int GetMiscOpx(void) const {
        return static_cast<unsigned>(get_u(disp_misc_opx, bits_per_misc_opx));
    }
    ALWAYS_INLINE unsigned int GetFusedOpx(void) const {
        return static_cast<unsigned>(get_u(disp_fused_opx, bits_per_fused_opx));
    }
    ALWAYS_INLINE unsigned int GetRaOpx(void) const {
        return static_cast<unsigned>(GetRegister(disp_reg_a));
    }
    ALWAYS_INLINE unsigned int GetBranchOpx(void) const {
        return static_cast<unsigned>(get_u(disp_branch_opx, bits_per_branch_opx));
    }

    friend std::ostream& operator<<(std::ostream& out, const CInsn1 cmd);

protected:
    ALWAYS_INLINE i64 get_s(EInstructionOffset disp, EInstructionSize size) const
        { return get_sbitfield(cmd, static_cast<unsigned>(disp), static_cast<unsigned>(size)); }
    ALWAYS_INLINE u64 get_u(EInstructionOffset disp, EInstructionSize size) const
        { return get_ubitfield(cmd, static_cast<unsigned>(disp), static_cast<unsigned>(size)); }

    void set_s(i64 value, EInstructionOffset disp, EInstructionSize size)
    {
        value &= util::makemask(size);
        cmd |= value << disp;
    }

    void set_u(u64 value, EInstructionOffset disp, EInstructionSize size)
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

protected:
    u64  cmd;
};

/***************************************************************************
* double-slot instruction
***************************************************************************/
class CInsn2 : public CInsn1 {
public:
    constexpr explicit CInsn2(u64 c, u64 e)
        : CInsn1(c), ext(e) {}

    u64 GetRawExtension(void) const {
        return ext;
    }

    i64 get_bin_simm(void) const {
        return get_sx(disp_binimm, bits_per_binimm,
                      disp_binimm_ext, bits_per_binimm_ext);
    }

    void set_bin_simm(i64 imm) {
        return set_sx(imm, disp_binimm, bits_per_binimm,
                           disp_binimm_ext, bits_per_binimm_ext);
    }

    u64 get_bin_uimm(void) const {
        return get_ux(disp_binimm, bits_per_binimm,
                      disp_binimm_ext, bits_per_binimm_ext);
    }

    void set_bin_uimm(u64 imm) {
        return set_ux(imm, disp_binimm, bits_per_binimm,
                           disp_binimm_ext, bits_per_binimm_ext);
    }

    i64 get_ldi(void) const {
        return get_sx(disp_call, bits_per_call,
                      disp_ldi_ext, bits_per_ldi_ext);
    }

    void set_ldi(i64 imm) {
        return set_sx(imm, disp_call, bits_per_call,
                           disp_ldi_ext, bits_per_ldi_ext);
    }

    i64 GetJumpDisplacement(void) const {
        return get_sx(disp_call, bits_per_call,
                      disp_call_ext, bits_per_call_ext);
    }

    void SetJumpDisplacement(i64 branch) {
        return set_sx(branch, disp_call, bits_per_call,
                              disp_call_ext, bits_per_call_ext);
    }

    i64 GetBranchDisplacement(void) const {
        return get_sx(disp_branch, bits_per_branch,
                      disp_branch_ext, bits_per_branch_ext);
    }

    void SetBranchDisplacement(i64 branch) {
        return set_sx(branch, disp_branch, bits_per_branch,
                              disp_branch_ext, bits_per_branch_ext);
    }

    ALWAYS_INLINE u64 ComputeJumpRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetJumpDisplacement();
    }

    u64 GetForwardBranch(void) const {
        return get_ux(disp_call, bits_per_call, disp_ldi_ext, bits_per_ldi_ext);
    }
    void SetForwardBranch(u64 branch) {
        return set_ux(branch, disp_call, bits_per_call,
                              disp_ldi_ext, bits_per_ldi_ext);
    }

    u64 ComputeForwardRelative(u64 ip) const {
        return ip + forward_offset_alignment * GetForwardBranch();
    }

    i64 get_brc_simm(void) const {
        return get_sx(disp_brci_imm, bits_per_brci_imm,
                      disp_brci_imm_ext, bits_per_brci_imm_ext); }

    void set_brc_simm(i64 imm) {
        return set_sx(imm, disp_brci_imm, bits_per_brci_imm,
                           disp_brci_imm_ext, bits_per_brci_imm_ext); }

    u64 get_brc_uimm(void) const {
        return get_ux(disp_brci_imm, bits_per_brci_imm,
                      disp_brci_imm_ext, bits_per_brci_imm_ext); }

    void set_brc_uimm(u64 imm) {
        return set_ux(imm, disp_brci_imm, bits_per_brci_imm,
                           disp_brci_imm_ext, bits_per_brci_imm_ext); }

    i64 GetIndexedDisplacement(void) const {
        return get_sx(disp_indexed_disp, bits_per_indexed_disp,
                      disp_indexed_disp_ext, bits_per_indexed_disp_ext);
    }

    void SetIndexedDisplacement(i64 disp) {
        return set_sx(disp, disp_indexed_disp, bits_per_indexed_disp,
                            disp_indexed_disp_ext, bits_per_indexed_disp_ext);
    }

    ALWAYS_INLINE u64 ComputeBranchRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetBranchDisplacement();
    }

    friend std::ostream& operator<<(std::ostream& out, const CInsn2 cmd);

private:
    i64 get_sx(EInstructionExtensionOffset disp, EInstructionExtensionSize size) const
    {
        return get_sbitfield(ext, static_cast<unsigned>(disp), static_cast<unsigned>(size));
    }

    u64 get_ux(EInstructionExtensionOffset disp, EInstructionExtensionSize size) const
    {
        return get_ubitfield(ext, static_cast<unsigned>(disp), static_cast<unsigned>(size));
    }

    i64 get_sx(EInstructionOffset disp, EInstructionSize size,
               EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size) const
    {
        return get_u(disp, size) | (get_sx(ext_disp, ext_size) << size);
    }

    u64 get_ux(EInstructionOffset disp, EInstructionSize size,
               EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size) const
    {
        return get_u(disp, size) | (get_ux(ext_disp, ext_size) << size);
    }

    void set_sx(i64 value, EInstructionExtensionOffset disp, EInstructionExtensionSize size)
    {
        value &= util::makemask(size);
        ext |= value << disp;
    }

    void set_ux(u64 value, EInstructionExtensionOffset disp, EInstructionExtensionSize size)
    {
        value &= util::makemask(size);
        ext |= value << disp;
    }

    void set_ux(u64 value, EInstructionOffset disp, EInstructionSize size,
                EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size)
    {
        set_u(value, disp, size);
        set_ux(value >> size, ext_disp, ext_size);
    }

    void set_sx(i64 value, EInstructionOffset disp, EInstructionSize size,
                EInstructionExtensionOffset ext_disp, EInstructionExtensionSize ext_size)
    {
        set_s(value, disp, size);
        set_sx(value >> size, ext_disp, ext_size);
    }

protected:
    u64  ext;
};


/***************************************************************************
* bundle of instructions
***************************************************************************/
enum class EBundleTemplate {
    bundle_sss = 0, // short, short, short
    bundle_sll = 1, // short, then long
    bundle_lls = 2, // long, then short
    bundle_lll = 3  // reserved
};

class CBundle {
public:
    explicit CBundle() : lo(0), hi(0) {}
    explicit CBundle(const u64 slot[slots_per_bundle], EBundleTemplate tmplt);

    u64 get_lo(void) const { return lo; }
    u64 get_hi(void) const { return hi; }
    u128 get_u128(void) const { return ((u128)hi << 64) | lo; }

    EBundleTemplate GetTemplate(void) const {
        return static_cast<EBundleTemplate>(lo & util::makemask(bits_per_bundle_template));
    }

    const CInsn1 s0(void) const { return CInsn1(GetSlotA()); }
    const CInsn1 s1(void) const { return CInsn1(GetSlotB()); }
    const CInsn1 s2(void) const { return CInsn1(GetSlotC()); }
    const CInsn2 l0(void) const { return CInsn2(GetSlotA(), GetSlotB()); }
    const CInsn2 l1(void) const { return CInsn2(GetSlotB(), GetSlotC()); }

    u128 v128(void) const
    {
        return (u128(hi) << 64) | lo;
    }

    friend std::ostream& operator<<(std::ostream& out, const CBundle& bundle);

    template<class Archive> void serialize(Archive& ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(lo);
        ar & BOOST_SERIALIZATION_NVP(hi);
    }
    static char const *bundle_template_2_str(EBundleTemplate tmplt);

private:
    u64 GetSlotA(void) const {
        return  (lo >> bits_per_bundle_template) & util::makemask(bits_per_slot);
    }
    u64 GetSlotB(void) const {
        return ((lo >> (bits_per_bundle_template + bits_per_slot)) |
                (hi << (64 - bits_per_slot - bits_per_bundle_template)))
               & util::makemask(bits_per_slot);
    }
    u64 GetSlotC(void) const {
        return  (hi >> (64 - bits_per_slot)) & util::makemask(bits_per_slot);
    }

private:
    u64   lo, hi; // assume little endian order (architecture dependent)
};

static_assert(sizeof(CBundle) == 16U);

} // namespace postrisc
