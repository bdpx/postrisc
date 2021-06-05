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
    CASE(ldubr) CASE(lduhr) CASE(lduwr) CASE(ldudr) CASE(ldqr)
    CASE(ldsbr) CASE(ldshr) CASE(ldswr) CASE(ldsdr)
    CASE(stbr) CASE(sthr) CASE(stwr) CASE(stdr) CASE(stqr)
    CASE(ldard) CASE(callplt) CASE(write)
        SetLabel(decoder, cmd, ip, cmd.ComputeForwardRelative(ip));
        break;

    CASE(ldarc) CASE(jmp) CASE(ehadj) CASE(call)
        SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip));
        break;

    CASE(ret) CASE(retf) CASE(callri) CASE(callmi) CASE(callrvt) CASE(syscall)
        // SetLabel(cmd, ip, ip);
        break;

    CASE(brbc) CASE(brbci)
    CASE(brbs) CASE(brbsi)
    CASE(ehnext)
    CASE(ehcatch)

    CASE(brmall)
    CASE(brmnotall)
    CASE(brmnone)
    CASE(brmany)

    CASE(breqw) CASE(breqiw)
    CASE(brnew) CASE(brneiw)
    CASE(breqd) CASE(breqid)
    CASE(brned) CASE(brneid)
    CASE(breqq) CASE(breqiq)
    CASE(brneq) CASE(brneiq)

    CASE(brltsw) CASE(brltsiw)
    CASE(brltuw) CASE(brltuiw)
    CASE(brgesw) CASE(brgesiw)
    CASE(brgeuw) CASE(brgeuiw)

    CASE(brltsd) CASE(brltsid)
    CASE(brltud) CASE(brltuid)
    CASE(brgesd) CASE(brgesid)
    CASE(brgeud) CASE(brgeuid)

    CASE(brltsq) CASE(brltsiq)
    CASE(brltuq) CASE(brltuiq)
    CASE(brgesq) CASE(brgesiq)
    CASE(brgeuq) CASE(brgeuiq)

    CASE(fbroeqh) CASE(fbroneh) CASE(fbrolth) CASE(fbrogeh) CASE(fbroh)
    CASE(fbrueqh) CASE(fbruneh) CASE(fbrulth) CASE(fbrugeh) CASE(fbruh)
    CASE(fbroeqs) CASE(fbrones) CASE(fbrolts) CASE(fbroges) CASE(fbros)
    CASE(fbrueqs) CASE(fbrunes) CASE(fbrults) CASE(fbruges) CASE(fbrus)
    CASE(fbroeqd) CASE(fbroned) CASE(fbroltd) CASE(fbroged) CASE(fbrod)
    CASE(fbrueqd) CASE(fbruned) CASE(fbrultd) CASE(fbruged) CASE(fbrud)
    CASE(fbroeqq) CASE(fbroneq) CASE(fbroltq) CASE(fbrogeq) CASE(fbroq)
    CASE(fbrueqq) CASE(fbruneq) CASE(fbrultq) CASE(fbrugeq) CASE(fbruq)

        SetLabel(decoder, cmd, ip, cmd.ComputeBranchRelative(ip));
        break;

    CASE(repltd) CASE(repltud) CASE(repled) CASE(repleud)
    CASE(repgtd) CASE(repgtud) CASE(repged) CASE(repgeud)
    CASE(repltw) CASE(repltuw) CASE(replew) CASE(repleuw)
    CASE(repgtw) CASE(repgtuw) CASE(repgew) CASE(repgeuw)

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
