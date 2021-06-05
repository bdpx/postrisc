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
    return (src >> disp) & util::makemask<u64>(size);
}

constexpr ALWAYS_INLINE i64 get_sbitfield(i64 src, unsigned int disp, unsigned int size)
{
    return (src << (64-disp-size)) >> (64-size);
}

/***************************************************************************
* single-slot instruction
***************************************************************************/
class InstructionSingle {
public:
    static const int SlotsNum = 1;

    constexpr explicit InstructionSingle(u64 c) : cmd(c) {}

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

    void set_shamt_b(unsigned int shamt) { SetShiftAmount(shamt, disp_reg_b); }
    void set_shamt_c(unsigned int shamt) { SetShiftAmount(shamt, disp_reg_c); }
    void set_shamt_d(unsigned int shamt) { SetShiftAmount(shamt, disp_reg_d); }
    void set_shamt_e(unsigned int shamt) { SetShiftAmount(shamt, disp_reg_e); }

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

    ALWAYS_INLINE i64 GetUpdateStride(void) const {
        return get_s(disp_update_stride, bits_per_update_stride);
    }
    void SetUpdateStride(i64 stride) {
        return set_s(stride, disp_update_stride, bits_per_update_stride);
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

    ALWAYS_INLINE i64 get_mid_bin_simm(void) const {
        return get_s(disp_mid_binimm, bits_per_mid_binimm);
    }
    void set_mid_bin_simm(i64 imm) {
        return set_s(imm, disp_mid_binimm, bits_per_mid_binimm);
    }

    template<typename T> T get_bin_imm(void) const;
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

    ALWAYS_INLINE i64 get_ld_imm(void) const {
        return get_s(disp_call, bits_per_call);
    }
    void set_ld_imm(i64 imm) {
        return set_s(imm, disp_call, bits_per_call);
    }

    ALWAYS_INLINE i64 GetJumpDisplacement(void) const {
        return get_s(disp_call, bits_per_call);
    }
    void SetJumpDisplacement(i64 branch) {
        return set_s(branch, disp_call, bits_per_call);
    }

    ALWAYS_INLINE u64 GetLoopStride(void) const {
        return get_u(disp_loop_stride, bits_per_loop_stride);
    }
    void SetLoopStride(u64 stride) {
        return set_u(stride, disp_loop_stride, bits_per_loop_stride);
    }

    // alloc/alloc.sp framesize
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

    ALWAYS_INLINE i64 GetLoopDisplacement(void) const {
        return get_s(disp_loop_disp, bits_per_loop_disp);
    }
    void SetLoopDisplacement(i64 branch) {
        return set_s(branch, disp_loop_disp, bits_per_loop_disp);
    }

    ALWAYS_INLINE u64 ComputeBranchRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetBranchDisplacement();
    }

    ALWAYS_INLINE u64 ComputeLoopRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetLoopDisplacement();
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
    ALWAYS_INLINE unsigned int GetIndexedOpx(void) const {
        return static_cast<unsigned>(get_u(disp_indexed_opx, bits_per_indexed_opx));
    }

    friend std::ostream& operator<<(std::ostream& out, const InstructionSingle insn)
    {
        return out << fmt::hex<u64, nibbles_per_slot>(insn.GetRaw());
    }

protected:
    ALWAYS_INLINE i64 get_s(EInstructionOffset disp, EInstructionSize size) const
        { return get_sbitfield(cmd, static_cast<unsigned>(disp), static_cast<unsigned>(size)); }
    ALWAYS_INLINE u64 get_u(EInstructionOffset disp, EInstructionSize size) const
        { return get_ubitfield(cmd, static_cast<unsigned>(disp), static_cast<unsigned>(size)); }

    void set_s(i64 value, EInstructionOffset disp, EInstructionSize size)
    {
        value &= util::makemask<u64>(size);
        cmd |= value << disp;
    }

    void set_u(u64 value, EInstructionOffset disp, EInstructionSize size)
    {
        value &= util::makemask<u64>(size);
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

protected:
    u64  cmd;
};


} // namespace postrisc
