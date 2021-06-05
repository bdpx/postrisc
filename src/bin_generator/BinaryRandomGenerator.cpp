#include <random>

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "BinaryRandomGenerator.hpp"

namespace postrisc {

BinaryRandomGenerator::BinaryRandomGenerator(const Decoder& decoder_)
    : Assembler(decoder_)
{
}

class RandomProvider {
public:
    RandomProvider() : distribution(0, std::numeric_limits<u64>::max())
    {}

    u64 operator() (void)
    {
        return distribution(generator);
    }

private:
    std::default_random_engine          generator;
    std::uniform_int_distribution<u64>  distribution;
};

int
BinaryRandomGenerator::AssembleGenerate(std::ostream& out, size_t codesize, size_t rodatasize, size_t datasize)
{
    Setup();

    ProgramSection& code   = GetSection(ESectionId_code);
    ProgramSection& rodata = GetSection(ESectionId_rodata);
    ProgramSection& data   = GetSection(ESectionId_data);

    code.global_size   = util::roundup<u64>(codesize, bits_per_segment_offset);
    rodata.global_size = util::roundup<u64>(rodatasize, bits_per_segment_offset);
    data.global_size   = util::roundup<u64>(datasize, bits_per_segment_offset);
    assert(code.global_size != 0);
    assert(rodata.global_size != 0);

    GetSection(0).global_offset = 0;
    GetSection(0).reserve(GetSection(0).global_size);
    for (size_t i = 1; i < GetSectionCount(); ++i) {
        ProgramSection& section = GetSection(i);
        section.global_offset = GetSection(i-1).global_offset + GetSection(i-1).global_size;
        section.reserve(section.global_size);
    }

    StartNewBundle();
    RandomProvider random;

    while (code.size() < code.global_size) {
        CompileRandomCode(random, code);
    }
    while (rodata.size() < rodata.global_size) {
        rodata.write( random() );
    }
    while (data.size() < data.global_size) {
        data.write( random() );
    }

    Write(out);
    return 0;
}

inline constexpr i64 signed_min(unsigned nbits)
{
     return - (I64C(1) << (nbits-1));
}

inline constexpr i64 signed_max(unsigned nbits)
{
     return (I64C(1) << (nbits-1)) - 1;
}

inline constexpr i64 unsigned_max(unsigned nbits)
{
     return (I64C(1) << nbits) - 1;
}

int
BinaryRandomGenerator::CompileRandomCode(RandomProvider& random, ProgramSection& section)
{
    int mnem = random() % insn_last;
    if (mnem == insn_undef || mnem == insn_illegal) {
       ++mnem;
    }

    const unsigned fmt = asm_info[mnem].sw_format;
    InstructionDouble cmd(asm_info[mnem].tmplt, 0);
    SetInstructionLength(cmd_short);

    const u64 ip = section.size();

    for (const EInstructionField field : asm_fmt_field[fmt]) {
        if (field == cf_none)
           break;

        const u64 r = random();

        switch (field) {

            case cf_comma:
            case cf_comma_opt:
            case cf_argsdelimiter:
                break;

            // optional '.l' (long) prefix for explicit double-width instruction emit
            case cf_lprefix:
                if (r % 24 == 2 && GetCurrentSlotNum() < 2) {
                    SetInstructionLength(cmd_long);
                }
                break;

            case cf_gpra:              cmd.set_ra(static_cast<unsigned>(r));                   break;
            case cf_gprb:              cmd.set_rb(static_cast<unsigned>(r));                   break;
            case cf_gprc:              cmd.set_rc(static_cast<unsigned>(r));                   break;
            case cf_gprd:              cmd.set_rd(static_cast<unsigned>(r));                   break;
            case cf_spr:               cmd.set_rs(static_cast<unsigned>(r % reg_spr_last));    break;
            case cf_framesize:         cmd.SetFrameSize(r % max_stacked_framesize);            break;
            case cf_shamt_b:           cmd.set_shamt_b(static_cast<unsigned>(r));              break;
            case cf_shamt_c:           cmd.set_shamt_c(static_cast<unsigned>(r));              break;
            case cf_shamt_d:           cmd.set_shamt_d(static_cast<unsigned>(r));              break;
            case cf_shamt_e:           cmd.set_shamt_e(static_cast<unsigned>(r));              break;
            case cf_mask_yes:          cmd.set_nullify_yes(r);                                 break;
            case cf_mask_no:           cmd.set_nullify_not(r);                                 break;
            case cf_indexed_scale:     cmd.SetIndexedScale(static_cast<unsigned>(r));          break;
            case cf_indexed_disp:      cmd.SetIndexedDisplacement(r);                          break;
            case cf_stride_simm:       cmd.SetUpdateStride(static_cast<unsigned>(r));          break;
            case cf_update_stride:     cmd.SetUpdateStride(static_cast<unsigned>(r));          break;

            case cf_brc_simm:
                cmd.set_brc_simm(r);
                break;

            case cf_brc_uimm:
                cmd.set_brc_uimm(r);
                break;

            case cf_bin_simm:
                cmd.set_bin_simm(r);
                break;

            case cf_bin_uimm:
                cmd.set_bin_uimm(r);
                break;

            // immediate for literal load
            case cf_ld_imm:
            case cf_ld_imm_f32:
            case cf_ld_imm_f64:
                cmd.set_ld_imm(r);
                SetInstructionLength(cmd_long);
                break;

            // rodata distances
            case cf_rip_string:
            case cf_rip_float32:
            case cf_rip_float64:
            case cf_rip_float128:
            case cf_rip_label: {
                //unsigned nbits = GetImmediateSize(bits_per_call, bits_per_call_long);
                u64 rr = util::roundup<u64>(GetSection(ESectionId_code).global_size, bits_per_segment_offset) - ip;
                auto xxx = GetSection(ESectionId_rodata).global_size;
                assert(xxx != 0);
                rr += (r % xxx);
                //uimm = GetLabel(ip, ESectionKind::text | ESectionKind::rodata | ESectionKind::data);
                cmd.SetForwardBranch(rr / forward_offset_alignment);
                }
                break;

            // branch distance for branch/loadaddress instructions
            case cf_call:
            case cf_lda_r:
                cmd.SetJumpDisplacement(
                    GetRandomBranchDisplacement(ip, r, bits_per_call, bits_per_call_long));
                break;

            // branch distance for compare-and-branch instructions
            case cf_branch:
                cmd.SetBranchDisplacement(
                    GetRandomBranchDisplacement(ip, r, bits_per_branch, bits_per_branch_long));
                break;

            default:
                LOG_ABORT << "unknown cf_field " << static_cast<int>(field);
        }
    } // next field

    if (GetInstructionLength() == cmd_long) {
        EmitInstruction(section, cmd.GetRaw(), cmd.GetRawExtension());
    } else {
        EmitInstruction(section, cmd.GetRaw());
    }

    return 0;
}

i64
BinaryRandomGenerator::GetRandomBranchDisplacement(u64 ip, u64 random, EInstructionSize size1, EInstructionFullSize size2)
{
    unsigned const nbits = GetImmediateSize(size1, size2);

    u64 const mod = GetSection(ESectionId_code).global_size - sizeof(Bundle);

    const i64 x = random % mod;
    LOG_DEBUG(LOG_PARSER) << "r=" << random << " mod=" << mod << " x=" << x;

    i64 rr = x - static_cast<i64>(ip);
    LOG_DEBUG(LOG_PARSER) << " rr=" << rr;

    rr /= static_cast<i64>(sizeof(Bundle));
    LOG_DEBUG(LOG_PARSER) << " rr=" << rr;

    rr = std::max<i64>(rr, signed_min(nbits));
    LOG_DEBUG(LOG_PARSER) << " rr=" << rr;

    rr = std::min<i64>(rr, signed_max(nbits));
    LOG_DEBUG(LOG_PARSER) << "x=" << x << " ip=" << ip << " rr=" << rr << " len=" << (int)GetInstructionLength();

    return rr;
}

} // namespace postrisc
