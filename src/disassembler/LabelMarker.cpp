#include "arch/isa.hpp"
#include "LabelMarker.hpp"
#include "Decoder.hpp"

namespace postrisc {

LabelMarker::LabelMarker(size_t size)
    : bitmap(size, false)
{
}

LabelMarker::~LabelMarker(void)
{
}

bool LabelMarker::is_label(size_t offset) const
{
    assert(offset < bitmap.size());
    return bitmap.at(offset);
}

/***************************************************************************
* this function mark address as branch target
***************************************************************************/
template<typename CMD>
void LabelMarker::SetLabel(const Decoder& decoder, const CMD cmd, const u64 ip, const u64 pos)
{
    LOG_DEBUG(LOG_PARSER) << "label: ip=" << ip << " pos=" << pos;

    if (pos < bitmap.size()) {
        bitmap[static_cast<size_t>(pos)] = true;
    } else {
        LOG_ABORT << InstructionDisassembler<CMD>(decoder, cmd, ip)
                  << "\ninvalid label offset " << fmt::dec(pos)
                  << "(" << fmt::dec(bitmap.size()) << "), ip=" << ip;
    }
}

/***************************************************************************
* this function check short instructions with ip-relative addressing.
***************************************************************************/
template<typename CMD>
void LabelMarker::MarkInstructionLabels(const Decoder& decoder, u64 ip, const CMD cmd)
{
#define CASE(NAME) case insn_##NAME:
    switch (decoder.Decode(cmd)) {
    CASE(ld_iprel_u8) CASE(ld_iprel_u16) CASE(ld_iprel_u32) CASE(ld_iprel_u64) CASE(ld_iprel_i128)
    CASE(ld_iprel_i8) CASE(ld_iprel_i16) CASE(ld_iprel_i32) CASE(ld_iprel_i64)
    CASE(st_iprel_i8) CASE(st_iprel_i16) CASE(st_iprel_i32) CASE(st_iprel_i64) CASE(st_iprel_i128)
    CASE(lda_iprel) CASE(call_plt) CASE(write)
        SetLabel(decoder, cmd, ip, cmd.ComputeForwardRelative(ip));
        break;

    CASE(lda_r) CASE(jmp) CASE(eh_adj) CASE(call)
        SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip));
        break;

    CASE(ret) CASE(retf) CASE(call_ri) CASE(call_mi) CASE(call_rvt) CASE(syscall)
        // SetLabel(cmd, ip, ip);
        break;

    CASE(br_bc)
    CASE(br_bs)
    CASE(br_bc_imm)
    CASE(br_bs_imm)
    CASE(eh_next)
    CASE(eh_catch)
    CASE(br_mask_all)
    CASE(br_mask_notall)
    CASE(br_mask_none)
    CASE(br_mask_any)

#define INT_CASE(ITYPE, UTYPE) \
    CASE(br_eq_imm_ ## ITYPE) \
    CASE(br_ne_imm_ ## ITYPE) \
    CASE(br_lt_imm_ ## ITYPE) \
    CASE(br_lt_imm_ ## UTYPE) \
    CASE(br_ge_imm_ ## ITYPE) \
    CASE(br_ge_imm_ ## UTYPE) \
    CASE(br_eq_     ## ITYPE) \
    CASE(br_ne_     ## ITYPE) \
    CASE(br_lt_     ## ITYPE) \
    CASE(br_ge_     ## ITYPE) \
    CASE(br_lt_     ## UTYPE) \
    CASE(br_ge_     ## UTYPE) \

    INT_CASE(i32,  u32)
    INT_CASE(i64,  u64)
    INT_CASE(i128, u128)

#define FPCASES(NAME) \
    CASE(br_oeq_   ## NAME) \
    CASE(br_one_   ## NAME) \
    CASE(br_olt_   ## NAME) \
    CASE(br_oge_   ## NAME) \
    CASE(br_o_     ## NAME) \
    CASE(br_ueq_   ## NAME) \
    CASE(br_une_   ## NAME) \
    CASE(br_ult_   ## NAME) \
    CASE(br_uge_   ## NAME) \
    CASE(br_u_     ## NAME) \

    FPCASES(f32)
    FPCASES(f64)
    FPCASES(f128)
        SetLabel(decoder, cmd, ip, cmd.ComputeBranchRelative(ip));
        break;

    CASE(rep_lt_i64) CASE(rep_lt_u64) CASE(rep_le_i64) CASE(rep_le_u64)
    CASE(rep_gt_i64) CASE(rep_gt_u64) CASE(rep_ge_i64) CASE(rep_ge_u64)
    CASE(rep_lt_i32) CASE(rep_lt_u32) CASE(rep_le_i32) CASE(rep_le_u32)
    CASE(rep_gt_i32) CASE(rep_gt_u32) CASE(rep_ge_i32) CASE(rep_ge_u32)

        SetLabel(decoder, cmd, ip, cmd.ComputeLoopRelative(ip));
        break;

    default:
        break;
    }
#undef CASE
}

void LabelMarker::MarkLabels(const Decoder& decoder, const Bundle bnd[], size_t codesize, size_t filesize)
{
    assert(filesize >= codesize);
    assert(codesize >= 1);

    // +1 for safe marking non-exist bundle after return or call in last bundle
    bitmap.assign(filesize+1, false);
    bitmap[0] = true;

    for (size_t ip = 0; ip < codesize; ip += sizeof(Bundle)) {
        Bundle const bundle = bnd[ip / sizeof(Bundle)];

        switch (bundle.GetTemplate()) {
        case EBundleTemplate::bundle_sss:
            MarkInstructionLabels(decoder, ip, bundle.s0());
            MarkInstructionLabels(decoder, ip, bundle.s1());
            MarkInstructionLabels(decoder, ip, bundle.s2());
            break;

        case EBundleTemplate::bundle_sll:
            MarkInstructionLabels(decoder, ip, bundle.s0());
            MarkInstructionLabels(decoder, ip, bundle.l1());
            break;

        case EBundleTemplate::bundle_lls:
            MarkInstructionLabels(decoder, ip, bundle.l0());
            MarkInstructionLabels(decoder, ip, bundle.s2());
            break;

        case EBundleTemplate::bundle_lll:
            LOG_ABORT << "invalid bundle template\n";
            break;
        }
    }
}

} // namespace postrisc
