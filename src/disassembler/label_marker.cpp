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
    CASE(ldbzr) CASE(ldhzr) CASE(ldwzr) CASE(lddzr) CASE(ldqr)
    CASE(ldbsr) CASE(ldhsr) CASE(ldwsr) CASE(lddsr)
    CASE(stbr) CASE(sthr) CASE(stwr) CASE(stdr) CASE(stqr)
    CASE(ldafr) CASE(callplt)
    CASE(write)
        SetLabel(decoder, cmd, ip, cmd.ComputeForwardRelative(ip));
        break;

    CASE(ldar) CASE(jmp) CASE(ehadj) CASE(callr)
        SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip));
        break;

    CASE(ret) CASE(callri) CASE(callmi) CASE(callmrw) CASE(callrvt) CASE(syscall)
        // SetLabel(cmd, ip, ip);
        break;

    CASE(bbc) CASE(bbs) CASE(bbci) CASE(bbsi)
    CASE(bdeq) CASE(bdne) CASE(bdlt) CASE(bdle) CASE(bdltu) CASE(bdleu)
    CASE(bweq) CASE(bwne) CASE(bwlt) CASE(bwle) CASE(bwltu) CASE(bwleu)
    CASE(ehnext) CASE(ehcatch)
    CASE(bdeqi) CASE(bdnei) CASE(bdlti) CASE(bdltui) CASE(bdgei) CASE(bdgeui)
    CASE(bweqi) CASE(bwnei) CASE(bwlti) CASE(bwltui) CASE(bwgei) CASE(bwgeui)
    CASE(bmall) CASE(bmnotall) CASE(bmnone) CASE(bmany)
    CASE(repdlt) CASE(repdltu) CASE(repdle) CASE(repdleu)
    CASE(repdgt) CASE(repdgtu) CASE(repdge) CASE(repdgeu)

#define FPCASES(NAME)           \
    CASE(bf ## NAME ## oeq)     \
    CASE(bf ## NAME ## one)     \
    CASE(bf ## NAME ## olt)     \
    CASE(bf ## NAME ## ole)     \
    CASE(bf ## NAME ## o)       \
    CASE(bf ## NAME ## ueq)     \
    CASE(bf ## NAME ## une)     \
    CASE(bf ## NAME ## ult)     \
    CASE(bf ## NAME ## ule)     \
    CASE(bf ## NAME ## uo)      \
    CASE(bf ## NAME ## class)

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

        default:
            LOG_ABORT << "invalid bundle template\n";
            break;
        }
    }
}

} // namespace postrisc
