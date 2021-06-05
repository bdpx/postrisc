#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "label_marker.hpp"
#include "decoder.hpp"

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
    return bitmap.at(offset);
}

/***************************************************************************
* this function mark address as branch target
***************************************************************************/
template<class T>
void
LabelMarker::SetLabel(const Decoder& decoder, const T cmd, const u64 ip, const u64 pos)
{
    LOG_DEBUG(LOG_PARSER) << "label: ip=" << ip << " pos=" << pos;

    if (pos < bitmap.size()) {
        bitmap[static_cast<size_t>(pos)] = true;
    } else {
        LOG_ABORT << CInstructionDisassembler<T>(decoder, cmd, ip)
                  << "\ninvalid label offset " << fmt::dec(pos)
                  << "(" << fmt::dec(bitmap.size()) << "), ip=" << ip;
    }
}

/***************************************************************************
* this function check short instructions with ip-relative addressing.
***************************************************************************/
template<class CMD>
void
LabelMarker::MarkInstructionLabels(const Decoder& decoder, u64 ip, const CMD cmd)
{
#define CASE(NAME) case insn_##NAME:
    switch (decoder.Decode(cmd)) {
    CASE(ldz_b_r) CASE(ldz_h_r) CASE(ldz_w_r) CASE(ldz_d_r) CASE(ld_q_r)
    CASE(lds_b_r) CASE(lds_h_r) CASE(lds_w_r) CASE(lds_d_r) CASE(st_b_r)
    CASE(st_h_r) CASE(st_w_r) CASE(st_d_r) CASE(st_q_r) CASE(ca_rf)
    CASE(call_plt) CASE(write)
        SetLabel(decoder, cmd, ip, cmd.ComputeForwardRelative(ip));
        break;

    CASE(ca_r) CASE(jmp) CASE(eh_adj) CASE(call_r)
        SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip));
        break;

    CASE(ret) CASE(retf) CASE(call_ri) CASE(call_mi) CASE(call_mrw) CASE(call_rvt) CASE(syscall)
        // SetLabel(cmd, ip, ip);
        break;

    CASE(b_bc) CASE(b_bs) CASE(b_bci) CASE(b_bsi)
    CASE(b_eq_d) CASE(b_ne_d) CASE(bs_lt_d) CASE(bs_le_d) CASE(bu_lt_d) CASE(bu_le_d)
    CASE(b_eq_w) CASE(b_ne_w) CASE(bs_lt_w) CASE(bs_le_w) CASE(bu_lt_w) CASE(bu_le_w)
    CASE(eh_next) CASE(eh_catch)
    CASE(bi_eq_d) CASE(bi_ne_d) CASE(bsi_lt_d) CASE(bui_lt_d) CASE(bsi_ge_d) CASE(bui_ge_d)
    CASE(bi_eq_w) CASE(bi_ne_w) CASE(bsi_lt_w) CASE(bui_lt_w) CASE(bsi_ge_w) CASE(bui_ge_w)
    CASE(bm_all) CASE(bm_notall) CASE(bm_none) CASE(bm_any)
    CASE(reps_lt_d) CASE(repu_lt_d) CASE(reps_le_d) CASE(repu_le_d)
    CASE(reps_gt_d) CASE(repu_gt_d) CASE(reps_ge_d) CASE(repu_ge_d)

#define FPCASES(NAME)       \
    CASE(bfo_eq_   ## NAME) \
    CASE(bfo_ne_   ## NAME) \
    CASE(bfo_lt_   ## NAME) \
    CASE(bfo_le_   ## NAME) \
    CASE(bfo_      ## NAME) \
    CASE(bfu_eq_   ## NAME) \
    CASE(bfu_ne_   ## NAME) \
    CASE(bfu_lt_   ## NAME) \
    CASE(bfu_le_   ## NAME) \
    CASE(bfu_      ## NAME) \
    CASE(bf_class_ ## NAME)

    FPCASES(ss)
    FPCASES(sd)
    FPCASES(sq)
        SetLabel(decoder, cmd, ip, cmd.ComputeBranchRelative(ip));
        break;

    default:
        break;
    }
#undef CASE
}

void
LabelMarker::MarkLabels(const Decoder& decoder, const CBundle bnd[], size_t codesize, size_t filesize)
{
    assert(filesize >= codesize);
    assert(codesize >= 1);

    // +1 for safe marking non-exist bundle after return or call in last bundle
    bitmap.assign(filesize+1, false);
    bitmap[0] = true;

    for (size_t ip = 0; ip < codesize; ip += sizeof(CBundle)) {
        CBundle const bundle = bnd[ip / sizeof(CBundle)];

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
