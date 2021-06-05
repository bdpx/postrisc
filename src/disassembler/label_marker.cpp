#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "fpu/float.hpp"
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
LabelMarker::SetLabel(const Decoder& decoder, const T cmd, const uint64_t ip, const uint64_t pos)
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
LabelMarker::MarkInstructionLabels(const Decoder& decoder, uint64_t ip, const CMD cmd)
{
    switch (decoder.Decode(cmd)) {
        case insn_ldr:
        case insn_ld1r:
        case insn_ld2r:
        case insn_ld4r:
        case insn_ld8r:
        case insn_ld16r:

        case insn_lds1r:
        case insn_lds2r:
        case insn_lds4r:
        case insn_lds8r:

        case insn_st1r:
        case insn_st2r:
        case insn_st4r:
        case insn_st8r:
        case insn_st16r:

        case insn_write:
            SetLabel(decoder, cmd, ip, cmd.ComputeForwardRelative(ip));
            break;

        case insn_ldar:
        case insn_br:
        case insn_alloc:
        case insn_ehadj:
        case insn_ehnext:
        case insn_ehcatch:
            SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip));
            break;

        case insn_callr:
            SetLabel(decoder, cmd, ip, ip);
            SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip));
            break;

        case insn_beqz:
        case insn_bnez:
        case insn_bltz:
        case insn_blez:
        case insn_bgtz:
        case insn_bgez:
            SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip));
            break;

        case insn_callplt:
            SetLabel(decoder, cmd, ip, ip);
            SetLabel(decoder, cmd, ip, cmd.ComputeForwardRelative(ip));
            break;

        case insn_ret:
        case insn_callri:
        case insn_callmi:
        case insn_syscall:
            SetLabel(decoder, cmd, ip, ip);
            break;

        case insn_bbc:
        case insn_bbs:
        case insn_bbci:
        case insn_bbsi:
        case insn_beq:
        case insn_bne:
        case insn_blt:
        case insn_ble:
        case insn_bltu:
        case insn_bleu:
            SetLabel(decoder, cmd, ip, cmd.ComputeBranchRelative(ip));
            break;

        case insn_beqi:
        case insn_bnei:
        case insn_blti:
        case insn_bgti:
        case insn_bltui:
        case insn_bgtui:
        case insn_replt:
        case insn_repltu:
        case insn_reple:
        case insn_repleu:
        case insn_repgt:
        case insn_repgtu:
        case insn_repge:
        case insn_repgeu:
            SetLabel(decoder, cmd, ip, cmd.ComputeShortBranchRelative(ip));
            break;

        default:
            break;
    }
}

void
LabelMarker::MarkLabels(const Decoder& decoder, const CBundle bnd[], size_t codesize, size_t filesize)
{
    assert(filesize >= codesize);
    assert(codesize >= 1);

    // +1 for safe marking non-exist bundle after return or call in last bundle
    bitmap.assign(filesize+1, false);
    bitmap[0] = true;

    for (size_t address = 0; address < codesize; address += sizeof(CBundle)) {
        CBundle const bundle = bnd[address / sizeof(CBundle)];
        uint64_t const ip = address + sizeof(CBundle);

        switch (bundle.GetTemplate()) {
            case EBundleTemplate::bundle_sss:
                MarkInstructionLabels(decoder, ip, bundle.s0());
                MarkInstructionLabels(decoder, ip, bundle.s1());
                MarkInstructionLabels(decoder, ip, bundle.s2());
                break;

            case EBundleTemplate::bundle_lls:
                MarkInstructionLabels(decoder, ip, bundle.l0());
                MarkInstructionLabels(decoder, ip, bundle.s2());
                break;

            case EBundleTemplate::bundle_sll:
                MarkInstructionLabels(decoder, ip, bundle.s0());
                MarkInstructionLabels(decoder, ip, bundle.l1());
                break;

            default:
                LOG_ABORT << "invalid bundle template\n";
                break;
        }
    }
}

} // namespace postrisc
