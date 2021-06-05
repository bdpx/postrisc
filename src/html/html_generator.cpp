#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"
#include "config/configuration.hpp"

namespace postrisc {

enum EInstructionHardwareType : u8 {
    instr_htype_sw,    // assembler pseudo-instructions
    instr_htype_hw,    // true hardware
    instr_htype_last
};

static const char inst_descr[][64] =
{
#define X(name, extname, format, descr) { descr },
#  include "arch/insn_table.hpp"
  { "last instruction" },
#undef X
#define NAME_X(name) { "extended group: " #name },
    DECLARE_OPCODES_WITH_EXTENDED_CODES(NAME_X)
};


/*****************************************************************************
* isa/asm html info
*****************************************************************************/

static const char field_type_cssclass[inst_fieldtype_last][4] =
{
#define INSN_FIELDTYPE_NAME_X(name, descr) #name,
    DECLARE_INST_FIELD_TYPES(INSN_FIELDTYPE_NAME_X)
};

static const char field_color_descr[][64] =
{
#define INSN_FIELDTYPE_DESCR_X(name, descr) descr,
    DECLARE_INST_FIELD_TYPES(INSN_FIELDTYPE_DESCR_X)
};


/******************************************************************************
* instruction fields data
******************************************************************************/
static const struct {
    EInstructionSize       size;
    EInstructionOffset     disp;
    EInstructionFieldKind  type;
    u8                  padding;
} inst_field_data[cf_last] =
{
#define INSTRUCTION_FIELD_DATA_X(field, size, disp, fieldtype, inst_name, descr) \
    { size, disp, inst_fieldtype_##fieldtype, 0 },

    DECLARE_INSTRUCTION_FIELDS(INSTRUCTION_FIELD_DATA_X)
};


static const char inst_field_name[cf_last][16] =
{
#define INSTRUCTION_FIELD_NAME_X(field, size, disp, fieldtype, inst_name, descr) inst_name,
    DECLARE_INSTRUCTION_FIELDS(INSTRUCTION_FIELD_NAME_X)
};


/******************************************************************************
* instructions data
******************************************************************************/

static const char inst_format_name[inst_fmt_last][16] =
{
#define INSTRUCTION_FORMAT_NAME_X(name, extname, descr, format, txt) #name,
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_NAME_X)
};

static const EInstructionField inst_fmt_field[inst_fmt_last][8] =
{
#define INSTRUCTION_FORMAT_FORMAT_X(name, extname, text, format, descr) format,
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_FORMAT_X)
};

/*
static const char inst_format_descr[] =
{
#define INSTRUCTION_FORMAT_DESCR_X(name, extname, bin_descr, format, descr) descr "\0"
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_DESCR_X)
};
*/

static const char instr_subset_name[instr_subset_last][12] =
{
#define INSTRUCTION_SUBSET_NAME_X(name) #name,
    DECLARE_INSTRUCTION_SUBSETS(INSTRUCTION_SUBSET_NAME_X)
};


static const char chapters[][32] =
{
    "instruction fields/arguments",
    "assembler instructions",
    "machine instruction formats",
    "machine instruction description",
    "register files",
    "machine instruction opcodes",
    "machine instruction statistic"
};

static void WriteColorMap(std::ostream& out)
{
    // hue(0-360, 0 is red, 120 is green, 240 is blue)
    // saturation (0-100%)
    // lightness (0-100%)

    std::array<unsigned, instr_subset_last> hues;
    for (size_t i = 0; i < hues.size(); i++) {
        hues[i] = (i * (360 / 20)) % 360;
    }

    out << "<style type=\"text/css\">\n";
    for (size_t i = 0; i < sizearray(instr_subset_name); i++) {
        out << "td." << instr_subset_name[i]
            << " {text-align:center; color:rgb(40,10,40); background-color:hsl("
            << hues[i] << ",90%,75%); }\n";
    }
    out << "</style>\n";
}

static std::string to_attribute(std::string input)
{
   //for (auto x: input) {
   //  x = (x == '.') ? '_' : x;
   //}
   return input;
}

static void chaptertitle(std::ostream& out, int num)
{
    out << "<h2 id=\"chapter_" << num << "\">" << chapters[num] << "</h2>\n";
}

static void registerfile(std::ostream& out, int sc,
    const Decoder::regname_array &names, const char descr[])
{
    int const size = 1 << bits_per_register;

    out << "<table width=\"95%\">\n"
           "<caption>" << descr << " (" << size << " from " << size << ")</caption>\n";

    for (int i = 0; i < size; i++) {
        if ((i & 7) == 0) out << "<tr>\n";
        out << "<td class=\"" << field_type_cssclass[sc] << "\">" << names[i].data() << "</td>\n";
        if ((i & 7) == 7) out << "</tr>\n";
    }

    out << "</table>\n"
           "<p></p>\n";
}

static void special_registerfile(std::ostream& out, int sc,
    const Decoder::regname_array &names, const char descr[])
{
    int const size = 1 << bits_per_register;

    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arch_spr_regtype[i] != ESpecialRegisterSubset::reserved) {
           count++;
        }
    }

    out << "<table width=\"95%\">\n"
           "<caption>" << descr << " (" << count << " from " << size << ")</caption>\n";

    for (int i = 0; i < size; i++) {
        if ((i & 7) == 0) out << "<tr>\n";

        if (arch_spr_regtype[i] != ESpecialRegisterSubset::reserved)
           out << "<td class=\"" << field_type_cssclass[sc] << "\">"
               << names[i].data() << " (" << i << ")</td>\n";
        else
           out << "<td class=\"res\">" << i << "</td>\n";

        if ((i & 7) == 7) out << "</tr>\n";
    }

    out << "</table>\n"
           "<p></p>\n";
}

template<size_t N> size_t
codetable(std::ostream& out,
    const Decoder::TInstructionMnemonicTable<N>& names,
    const char *anchor,
    const char *descr)
{
    size_t imp = 0, maxint = 0;
    size_t const max_name_len = 24;
    const size_t row_size = 4;

    const size_t mask = std::min(N, row_size) - 1;

    for (size_t i = 0; i < N; i++) {
        if (names[i] != insn_illegal) {
           imp++;
           maxint = i;
        }
    }
    maxint = (maxint + row_size) & ~(row_size - 1);

    out <<  "<div id=\"" << anchor << "_table\"></div>\n"
            "<table width=\"100%\">\n"
            "<caption>" << descr << " (" << imp << " from " << N << ")</caption>\n";

    for (size_t i = 0; i < N; i++) {
        if ((i & mask) == 0)
           out << "<tr>\n";
        const EInstructionID mnem = names[i];
        if (mnem != insn_illegal) {
           EInstructionSubset subset = asm_info[mnem].subset;
           out << "<td class=\"" << instr_subset_name[subset] << "\">";
           out << "<a href=\"#" << to_attribute(insn_name(mnem));
           if (mnem >= insn_last)
              out << "_table";
           out << "\">";
           size_t const len = strlen(insn_name(mnem));
           if (len > max_name_len) {
             out << std::string_view(insn_name(mnem), max_name_len) << "&hellip;";
           } else {
             out << insn_name(mnem);
           }
           out << "</a> " << i << "</td>\n";
        } else {
           out << "<td class=\"res\">reserved " << i << "</td>\n";
        }
        if ((i & mask) == mask)
           out << "</tr>\n";
    }

    out << "</table>\n"
           "<p></p>\n";
    return (imp);
}


/*************************************************************************
* this function print html assembler help
*************************************************************************/
int Decoder::GenerateHtml(std::ostream& out)
{
    unsigned int chapter = 0;

    xhtml_header(out, ARCH_NAME " virtual processor instruction set",
                      ARCH_NAME " virtual processor instruction set generated manual", WriteColorMap);

    // content
    out << "<ul>\n";
    for (size_t i = 0; i < sizearray(chapters); i++) {
        out << "<li><a href=\"#chapter_" << i << "\">" << chapters[i] << "</a></li>\n";
    }
    out << "</ul>\n";

    // instructions fields/arguments
    chaptertitle(out, chapter++);

    out << "<ul>\n"
         "<li>opcode - main operation code</li>\n"
         "<li>opx - extended operation code</li>\n"
         "<li>ra,rb,rc,rd - general purpose registers r0&hellip;r119, g0&hellip;g3, tp, fp, sp, gz</li>\n"
         "<li>spr - special purpose registers (psr, itc, fpsr, &hellip;)</li>\n"
         "<li>sb,sc,se,sd - shift value 0&hellip;63</li>\n"
         "<li>vcount,andmask,ormask - value 0&hellip;127</li>\n"
         "<li>simmx(y) - x (or y) bit length immediate (sign extended to 64 bit value)</li>\n"
         "<li>uimmx(y) - x (or y) bit length immediate (zero extended to 64 bit value)</li>\n"
         "<li>simmNx16 - N bit length code label (16 byte aligned)</li>\n"
         "<li>scale - 3-bit scaling shift value</li>\n"
         "<li>dy,dn - predication block size for `yes` and `no` variants</li>\n"
         "<li>framesize - register window size</li>\n"
         "<li>sext,zext - sign/zero extended</li>\n"
         "<li>* - instruction alias</li>\n"
         "</ul>\n";

    /****************************************************************************
    * assembler instructions
    ****************************************************************************/

    chaptertitle(out, chapter++);
    out <<
        "<table>\n"
        "<thead>\n"
        "<tr>\n"
        "<th>group</th>\n"
        "<th>syntax</th>\n"
        "<th>description</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tbody>\n";

    for (const auto & insn : asm_names) {
        const EInstructionID inst = insn.second;
        const EInstructionSubset subset = asm_info[inst].subset;
        out << "<tr><td>" << instr_subset_name[subset] << "</td>"
               "<td><pre><a href=\"#" << to_attribute(insn_name(inst)) << "\">" << insn_name(inst) << "</a>";

        const EInstructionAssemblerFormat format = asm_info[inst].sw_format;

        for (const EInstructionField field : asm_fmt_field[format]) {
            if (field == cf_none)
                break;
            switch (field) {
            case cf_lprefix:
                out << '.';
                break;
            case cf_argsdelimiter:
                out << ' ';
                break;
            default:
                break;
            }
            out << inst_field_name[field];
        }
        out << "</pre></td><td>" << inst_descr[inst];

        EInstructionID realInsnId = Decode(InstructionSingle(asm_info[inst].tmplt));
        if (inst != realInsnId) {
            out << "*";
        }

        out << "</td></tr>\n";
    }
    out << "</tbody>\n"
           "</table>\n\n";

    /****************************************************************************
    * machine instruction formats
    ****************************************************************************/

    const size_t table_width = 99; // percent
    const size_t name_width = 20; // percents

    chaptertitle(out, chapter++);

    // bundle formats
    out <<
        "<table width=\"86%\">\n"
        "<caption>bundle formats:</caption>\n"
        "<thead><tr>\n"
        "<th>slot 3<br/>(" << bits_per_slot << " bits)</th>"
        "<th>slot 2<br/>(" << bits_per_slot << " bits)</th>"
        "<th>slot 1<br/>(" << bits_per_slot << " bits)</th>"
        "<th style=\"width:12%\">template<br/>(" << bits_per_bundle_template << " bits)</th>\n"
        "</tr></thead><tbody>\n"
        "<tr><td class=\"gpr\">short 3</td>"
          "<td class=\"gpr\">short 2</td>"
          "<td class=\"gpr\">short 1</td>"
          "<td class=\"imm\">00</td></tr>\n"
        "<tr><td class=\"tdb\" colspan=\"2\">long 2</td>"
          "<td class=\"gpr\">short 1</td>"
          "<td class=\"imm\">01</td></tr>\n"
        "<tr><td class=\"gpr\">short 2</td>"
          "<td class=\"tdb\" colspan=\"2\">long 1</td>"
          "<td class=\"imm\">10</td></tr>\n"
        "<tr><td class=\"res\" colspan=\"3\">very long</td>"
          "<td class=\"imm\">11</td></tr>\n"
        "</tbody></table>\n";

    out << "<table width=\"" << table_width << "%\">\n"
        "<caption>slot formats:</caption>\n"
        "<tr>\n"
        "<td class=\"bit\" rowspan=\"2\" style=\"width:" << name_width << "%\">format<br/>name</td>\n"
        "<td class=\"bit\" colspan=\"42\">bit numbers</td>\n"
        "</tr>\n"

    "<tr>\n";
    for (int pos = bits_per_slot - 1; pos >= 0; pos--) {
        out << "<td class=\"bit\">" << pos << "</td>\n";
    }
    out << "</tr>\n";

    for (size_t format = 0; format < sizearray(inst_fmt_field); format++) {
        out << "<tr><td>" << inst_format_name[format] << "</td>\n"
            "<td class=\"" << field_type_cssclass[inst_fieldtype_opc]
            << "\" colspan=\"" << static_cast<unsigned>(bits_per_opcode) << "\">opcode</td>\n";

        for (size_t pos=disp_opcode, i=0; pos>0 && i < sizearray(inst_fmt_field[0]); i++) {
            const EInstructionField     field      = inst_fmt_field[format][i];
            const EInstructionSize      field_size = inst_field_data[field].size;
            const EInstructionFieldKind field_type = inst_field_data[field].type;
            if (field_size > 0) {
               pos -= field_size;
               out << "<td class=\"" << field_type_cssclass[field_type]
                   << "\" colspan=\"" << static_cast<unsigned>(field_size) << "\">"
                   << inst_field_name[field] << "</td>\n";
               //assert(inst_field_data[field].disp == disp_unknown ||
               //       inst_field_data[field].disp == pos);
            }
        }
        out << "</tr>\n";
    }
    out << "</table>\n";

    out << "<table width=\"60%\">\n"
        "<caption>color descriptions for instruction fields:</caption>\n";

    for (int i = 0; i < inst_fieldtype_last; i++) {
        out << "<tr><td class=\"" << field_type_cssclass[i] << "\">"
            << field_color_descr[i] << "</td></tr>\n";
    }
    out << "</table>\n";

    /****************************************************************************
    * assembler instructions bit-formatted list
    ****************************************************************************/

    chaptertitle(out, chapter++);
    out << "<table width=\"" << table_width << "%\">\n"
           "<tr>\n"
           "<td class=\"bit\" rowspan=\"2\" style=\"width:" << name_width << "%\">instruction<br/>mnemonic</td>\n"
           "<td class=\"bit\" colspan=\"" << static_cast<unsigned>(bits_per_slot)
           << "\">bit numbers</td></tr>\n"
           "<tr>\n";

    for (int pos = bits_per_slot - 1; pos >= 0; pos--) {
        out << "<td class=\"bit\">" << pos << "</td>\n";
    }
    out << "</tr>\n";

    for (const auto & insn : asm_names) {
        const EInstructionID inst = insn.second;
        const u64 cmd = asm_info[inst].tmplt;
        const auto format = asm_info[inst].hw_format;
        const unsigned opcode = InstructionSingle(cmd).GetOpcode();

        out << "<tr>\n"
               "<td><div id=\"" << to_attribute(insn_name(inst)) << "\"></div>" << insn_name(inst) << "</td>\n";
        out << "<td class=\"" << field_type_cssclass[inst_fieldtype_opc]
            << "\" colspan=\"" << static_cast<unsigned>(bits_per_opcode)
            << "\"><a href=\"#opcode_table\">" << opcode << "</a></td>\n";

        for (int pos = disp_opcode, i = 0; pos > 0; i++) {
            const EInstructionField     field      = inst_fmt_field[format][i];
            const EInstructionSize      field_size = inst_field_data[field].size;
            const EInstructionFieldKind field_type = inst_field_data[field].type;
            if (field_size > 0) {
               pos -= field_size;
               out << "<td class=\"" << field_type_cssclass[field_type]
                   << "\" colspan=\"" << static_cast<unsigned>(field_size) << "\">";

               if (field_type == inst_fieldtype_opx) {
                  unsigned const opx = static_cast<unsigned>(get_ubitfield(cmd, pos, field_size));
                  out << "<a href=\"#" << to_attribute(insn_name(arch_info_opcode[opcode]))
                      << "_table\">" << opx << "</a>";
               } else {
                  out << inst_field_name[field];
               }
               out << "</td>\n";
            }
        }
        out << "</tr>\n";
    }
    out << "</table>\n";

    chaptertitle(out, chapter++);
    registerfile(out, inst_fieldtype_gpr, Decoder::gpr_names, "128-bit general purpose registers");
    special_registerfile(out, inst_fieldtype_spr, Decoder::spr_names, "64-bit special purpose registers");

    /****************************************************************************
    * instruction encoding
    ****************************************************************************/

    chaptertitle(out, chapter++);
    const size_t opcodes = codetable(out, arch_info_opcode, "opcode", "<strong>Primary opcodes</strong>");

    int num_opcodes_with_extcode = 0;
    const size_t opx_size = 32; // 1(opcodes) + 17(all extcodes)
    EInstructionID opc_name[opx_size];
    size_t opx_num[opx_size];

#define OPC_TABLE(NAME)                                               \
    opc_name[num_opcodes_with_extcode] = insn_##NAME;                 \
    opx_num[num_opcodes_with_extcode++] = codetable                   \
          (out, arch_info_ ## NAME, insn_name(insn_##NAME),           \
                "The &laquo;<strong>" #NAME "</strong>&raquo; extended opcodes");

    OPC_TABLE(br_misc)
    OPC_TABLE(br_32)
    OPC_TABLE(br_64)
    OPC_TABLE(br_128)
    OPC_TABLE(fused)
    OPC_TABLE(nul_misc)
    OPC_TABLE(nul_32)
    OPC_TABLE(nul_64)
    OPC_TABLE(nul_128)
    OPC_TABLE(fma)
    OPC_TABLE(mem_xi64)
    OPC_TABLE(mem_xu64)
    OPC_TABLE(mem_xi32)
    OPC_TABLE(mem_xu32)
    OPC_TABLE(loop)
    OPC_TABLE(raopx)
    OPC_TABLE(misc)

    unsigned snum[instr_subset_last][instr_htype_last] = { { 0 } };
    unsigned snum_sum[instr_htype_last] = { 0 };

    for (int inst = 0; inst < insn_illegal; inst++) {
        EInstructionSubset subset = asm_info[inst].subset;
        EInstructionID realInsnId = Decode(InstructionSingle(asm_info[inst].tmplt));
        if (inst == realInsnId || subset == instr_subset_group)
            snum[subset][instr_htype_hw]++;
        else
            snum[subset][instr_htype_sw]++;
    }

    for (int subset = 0; subset < instr_subset_last; subset++) {
        for (int hard = 0; hard < instr_htype_last; hard++) {
            snum_sum[hard] += snum[subset][hard];
        }
    }
    assert(snum_sum[instr_htype_hw] + snum_sum[instr_htype_sw] == insn_illegal);

    /*****************************************************************************
    * instruction subset encoding statistic
    *****************************************************************************/

    chaptertitle(out, chapter++);
    out <<
        "<table width=\"60%\">\n"
        "<caption>statistic by instruction subsets:</caption>\n"
        "<thead>\n"
        "<tr>\n"
        "<th>instruction subset</th>\n"
        "<th>all</th>\n"
        "<th>hardwired</th>\n"
        "<th>pseudo-ops</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tfoot>\n"
        "<tr>\n"
        "<th>sum:</th>\n"
        "<th>" << (snum_sum[instr_htype_hw] + snum_sum[instr_htype_sw]) << "</th>\n"
        "<th>" << snum_sum[instr_htype_hw] << "</th>\n"
        "<th>" << snum_sum[instr_htype_sw] << "</th>\n"
        "</tr>\n"
        "</tfoot>\n"
        "<tbody>\n";

    for (int subset = 0; subset < instr_subset_last; subset++) {
        out << "<tr>\n"
               "<td class=\"" << instr_subset_name[subset] << "\">"
            << instr_subset_name[subset] << "</td>\n"
               "<td class=\"cnt\">"
            << (snum[subset][instr_htype_hw] + snum[subset][instr_htype_sw]) << "</td>\n"
               "<td class=\"cnt\">" << snum[subset][instr_htype_hw] << "</td>\n"
               "<td class=\"cnt\">" << snum[subset][instr_htype_sw] << "</td>\n"
               "</tr>\n";
    }

    out << "</tbody>\n"
           "</table>\n"
           "<p></p>\n";

    /*****************************************************************************
    * extended opcode statistic
    *****************************************************************************/
    int extcodes = 0;
    for (int i = 0; i < num_opcodes_with_extcode; i++) {
        extcodes += opx_num[i];
    }

    const int opc_with_opx_num = insn_illegal - insn_last;

    out <<
        "<table width=\"60%\">\n"
        "<caption>statistic by instruction opcodes ("
        << (opcodes + extcodes - opc_with_opx_num) << " codes, "
        << opc_with_opx_num << " groups):</caption>\n"
        "<thead>\n"
        "<tr>\n"
        "<th>opcode</th>\n"
        "<th>num</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tbody>\n"
        "<tr>\n"
        "<td class=\"cnt\">primary opcodes</td>\n"
        "<td class=\"cnt\">" << opcodes << "</td>\n"
        "</tr>\n";

    for (int i = 0; i < num_opcodes_with_extcode; i++) {
        out << "<tr>\n"
               "<td class=\"cnt\">" << insn_name(opc_name[i]) << "</td>\n"
               "<td class=\"cnt\">" << opx_num[i] << "</td>\n"
               "</tr>\n";
    }
    out << "</tbody>\n"
           "</table>\n";

    out << "<p></p>\n"
           "<hr/>\n"
           "<p class=\"copyright\">" ARCH_NAME " virtual processor</p>\n"
           "<p class=\"copyright\">Instruction Set Architecture (ISA) reference manual.</p>\n"
           "<p class=\"copyright\">" ARCH_COPYRIGHT "</p>\n"
           "<p class=\"copyright\">This file was generated automatically.</p>\n"
           "</body>\n"
           "</html>\n";

    return 0;
}

} // namespace postrisc


int main(void)
{
    postrisc::util::setup();

    postrisc::Decoder decoder;
    return decoder.GenerateHtml(std::cout);
}
