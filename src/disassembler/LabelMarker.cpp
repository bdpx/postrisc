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
bool LabelMarker::SetLabel(const Decoder& decoder, const CMD cmd, const u64 ip, const u64 pos)
{
    LOG_DEBUG(LOG_DECODER) << "label: ip=" << ip << " pos=" << pos;

    if (pos < bitmap.size()) {
        bitmap[static_cast<size_t>(pos)] = true;
        return true;
    } else {
        LOG_ERROR(LOG_DECODER) << InstructionDisassembler<CMD>(decoder, cmd, ip)
                  << "\ninvalid label offset " << fmt::dec(pos)
                  << "(" << fmt::dec(bitmap.size()) << "), ip=" << ip;
        return false;
    }
}

/***************************************************************************
* this function check short instructions with ip-relative addressing.
***************************************************************************/
template<typename CMD>
bool LabelMarker::MarkInstructionLabels(const Decoder& decoder, u64 ip, const CMD cmd)
{
    const EInstructionID mnem = decoder.Decode(cmd);
    const EInstructionAssemblerFormat fmt = asm_info[mnem].sw_format;

    for (const EInstructionField field : asm_fmt_field[fmt]) {
        switch (field) {
            case cf_none:
                return true;
            case cf_rip_string:
            case cf_rip_label:
                if (!SetLabel(decoder, cmd, ip, cmd.ComputeForwardRelative(ip)))
                    return false;
                break;
            case cf_ldarc:
            case cf_call:
                if (!SetLabel(decoder, cmd, ip, cmd.ComputeJumpRelative(ip)))
                    return false;
                break;
            case cf_branch:
                if (!SetLabel(decoder, cmd, ip, cmd.ComputeBranchRelative(ip)))
                    return false;
                break;
            case cf_loop_disp:
                if (!SetLabel(decoder, cmd, ip, cmd.ComputeLoopRelative(ip)))
                    return false;
                break;
            default:
                break;
        }
    }
    return true;
}

bool LabelMarker::MarkLabels(const Decoder& decoder, const Bundle bnd[], size_t codesize, size_t filesize)
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
            if (!MarkInstructionLabels(decoder, ip, bundle.s0())) return false;
            if (!MarkInstructionLabels(decoder, ip, bundle.s1())) return false;
            if (!MarkInstructionLabels(decoder, ip, bundle.s2())) return false;
            break;

        case EBundleTemplate::bundle_sll:
            if (!MarkInstructionLabels(decoder, ip, bundle.s0())) return false;
            if (!MarkInstructionLabels(decoder, ip, bundle.l1())) return false;
            break;

        case EBundleTemplate::bundle_lls:
            if (!MarkInstructionLabels(decoder, ip, bundle.l0())) return false;
            if (!MarkInstructionLabels(decoder, ip, bundle.s2())) return false;
            break;

        case EBundleTemplate::bundle_lll:
            LOG_ERROR(LOG_DECODER) << "invalid bundle template\n";
            return false;
        }
    }
    return true;
}

} // namespace postrisc
