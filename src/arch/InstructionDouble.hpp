#pragma once

#include "InstructionSingle.hpp"

namespace postrisc {

/***************************************************************************
* double-slot instruction
***************************************************************************/
class InstructionDouble : public InstructionSingle {
public:
    static const int SlotsNum = 2;

    constexpr explicit InstructionDouble(u64 c, u64 e)
        : InstructionSingle(c), ext(e) {}

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

    i64 get_mid_bin_simm(void) const {
        return get_sx(disp_mid_binimm, bits_per_mid_binimm,
                      disp_mid_binimm_ext, bits_per_mid_binimm_ext);
    }

    void set_mid_bin_simm(i64 imm) {
        return set_sx(imm, disp_mid_binimm, bits_per_mid_binimm,
                           disp_mid_binimm_ext, bits_per_mid_binimm_ext);
    }

    i64 get_ld_imm(void) const {
        return get_sx(disp_call, bits_per_call,
                      disp_ld_imm_ext, bits_per_ld_imm_ext);
    }

    void set_ld_imm(i64 imm) {
        return set_sx(imm, disp_call, bits_per_call,
                           disp_ld_imm_ext, bits_per_ld_imm_ext);
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

    ALWAYS_INLINE u64 GetLoopStride(void) const {
        return get_ux(disp_loop_stride, bits_per_loop_stride,
                      disp_loop_stride_ext, bits_per_loop_stride_ext);
    }

    void SetLoopStride(u64 stride) {
        return set_ux(stride, disp_loop_stride, bits_per_loop_stride,
                              disp_loop_stride_ext, bits_per_loop_stride_ext);
    }

    ALWAYS_INLINE i64 GetLoopDisplacement(void) const {
        return get_sx(disp_loop_disp, bits_per_loop_disp,
                      disp_loop_disp_ext, bits_per_loop_disp_ext);
    }

    void SetLoopDisplacement(i64 branch) {
        return set_sx(branch, disp_loop_disp, bits_per_loop_disp,
                              disp_loop_disp_ext, bits_per_loop_disp_ext);
    }

    ALWAYS_INLINE u64 ComputeJumpRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetJumpDisplacement();
    }

    u64 GetForwardBranch(void) const {
        return get_ux(disp_call, bits_per_call, disp_ld_imm_ext, bits_per_ld_imm_ext);
    }
    void SetForwardBranch(u64 branch) {
        return set_ux(branch, disp_call, bits_per_call,
                              disp_ld_imm_ext, bits_per_ld_imm_ext);
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

    ALWAYS_INLINE u64 ComputeLoopRelative(u64 ip) const {
        return ip + bytes_per_bundle * GetLoopDisplacement();
    }

    friend std::ostream& operator<<(std::ostream& out, const InstructionDouble insn)
    {
        return out << fmt::hex<u64, nibbles_per_slot>(insn.GetRaw())
                   << ' '
                   << fmt::hex<u64, nibbles_per_slot>(insn.GetRawExtension());
    }

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
        value &= util::makemask<u64>(size);
        ext |= value << disp;
    }

    void set_ux(u64 value, EInstructionExtensionOffset disp, EInstructionExtensionSize size)
    {
        value &= util::makemask<u64>(size);
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

} // namespace postrisc
