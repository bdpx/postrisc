#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "html_generator.hpp"
#include "disassembler/decoder.hpp"

/***************************************************************************
* assembler instruction description (see isa.h for macrotable)
* compact and romable strings representation for sequential access
***************************************************************************/

namespace postrisc {

static const char inst_descr[][56] =
{
#define X(name, extname, inst_format, format, iic, tmplt, descr) descr,
#  include "arch/insn_table.hpp"
  "last instruction"
#undef X
#define NAME_X(name) #name,
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
    uint8_t                  padding;
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

static const char inst_format_name[inst_fmt_last][10] =
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

static const char instr_subset_name[instr_subset_last][8] =
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

void xhtml_header(std::ostream& out, const char *title)
{
    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
        "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
        "<head>\n";

    if (title)
        out << "<title>" << title << "</title>\n";
    else
        out << "<title>Piped file</title>\n";

    out << "<meta http-equiv=\"content-type\" content=\"application/xhtml+xml; charset=utf-8\"/>\n"
        "<meta http-equiv=\"content-style-type\" content=\"text/css\"/>\n"
        "<meta name=\"language\" content=\"en\"/>\n"
        "<meta name=\"viewport\" content=\"width=device-width\"/>\n"
        "<link href=\"styles.css\" rel=\"stylesheet\" type=\"text/css\"/>\n"
        "</head>\n"
        "<body>\n";

    if (title)
        out << "<h1>" << title << "</h1>\n";

/*
<p><a href="http://validator.w3.org/check?uri=referer">
<img src="http://www.w3.org/Icons/valid-xhtml11" alt="Valid XHTML 1.1" height="31" width="88"/></a></p>
*/
}

static void chaptertitle(std::ostream& out, int num)
{
    out << "<a id=\"chapter_" << num << "\"></a>\n"
           "<h2>" << chapters[num] << "</h2>\n";
}


static void
registerfile(std::ostream& out, int sc, const Decoder::register_names_table & names, const char descr[])
{
    int    i, count, size;

    size = 1 << bits_per_register;

    for (count = 0, i = 0; i < size; i++) {
        if (names[i][0] != '\0') {
           count++;
        }
    }

    out << "<table width=\"80%\">\n"
           "<caption>" << descr << " (" << count << " from " << size << ")</caption>\n";

    for (i = 0; i < size; i++) {
        if ((i & 7) == 0)
           out << "<tr>\n";

        if (names[i][0] != '\0')
           out << "\t<td class=\"" << field_type_cssclass[sc] << "\">" << names[i] << "<br/>" << i << "</td>\n";
        else
           out << "\t<td class=\"res\">reserved<br/>" << i << "</td>\n";

        if ((i & 7) == 7)
           out << "</tr>\n";
    }

    out << "</table>\n"
           "<br/>\n";
}


template<int N> int
codetable(std::ostream& out,
    const Decoder::TInstructionMnemonicTable<N>& names,
    const char *anchor,
    const char *descr)
{
    int imp = 0, maxint = 0;

    const int mask = std::min(N, 8) - 1;

    for (int i = 0; i < N; i++) {
        if (names[i] != insn_undef) {
           imp++;
           maxint = i;
        }
    }
    maxint = (maxint + 8) & ~7;

    out <<  "<a id=\"" << anchor << "_table\"></a>\n"
            "<table width=\"92%\">\n"
            "<caption>" << descr << " (" << imp << " from " << N << ")</caption>\n";

    for (int i = 0; i < maxint; i++) {
        if ((i & mask) == 0)
           out << "<tr>\n";
        const EInstructionID mnem = names[i];
        if (mnem != insn_undef) {
           EInstructionSubset subset = asm_info[mnem].subset;
           out << "\t<td class=\"" << instr_subset_name[subset] << "\">";
           out << "<a href=\"#" << asm_name[mnem];
           if (mnem >= insn_last)
              out << "_table";
           out << "\">" << asm_name[mnem] << "</a><br/>" << i << "</td>\n";
        } else {
           out << "\t<td class=\"res\">reserved<br/>" << i << "</td>\n";
        }
        if ((i & mask) == mask)
           out << "</tr>\n";
    }

    out << "</table>\n"
           "<br/>\n";
    return (imp);
}


/*************************************************************************
* this function print html assembler help
*************************************************************************/
int arch_html(const Decoder& decoder, std::ostream& out)
{
    unsigned int chapter = 0;

    xhtml_header(out, ARCH_NAME " virtual processor instruction set");

    // content
    out << "<ul>\n";
    for (size_t i = 0; i < sizearray(chapters); i++) {
        out << "\t<li><a href=\"#chapter_" << i << "\">" << chapters[i] << "</a></li>\n";
    }
    out << "</ul>\n";

    // instructions fields/arguments
    chaptertitle(out, chapter++);

    out << "<ul>\n"
         "\t<li>ra,rb,rc,rd - general purpose registers r0&hellip;r111, sp, fp, tp, g3 &hellip; g15</li>\n"
         "\t<li>spr - special purpose registers (psr, itc, fpsr, &hellip;)</li>\n"
         "\t<li>sb,sc,se,sd - shift value 0&hellip;63</li>\n"
         "\t<li>vcount,andmask,ormask - value 0&hellip;127</li>\n"
         "\t<li>simmx(y) - x (or y) bit length immediate (sign extended to 64 bit value)</li>\n"
         "\t<li>uimmx(y) - x (or y) bit length immediate (zero extended to 64 bit value)</li>\n"
         "\t<li>simmNx16 - label defined anywhere (N bits length)</li>\n"
         "\t<li>uimm27 - forward label (data only)</li>\n"
         "\t<li>scale - 3-bit scaling shift value</li>\n"
         "\t<li>mo - memory ordering</li>\n"
         "\t<li>rm - rounding mode</li>\n"
         "\t<li>* - instruction alias</li>\n"
         "</ul>\n";

    /****************************************************************************
    * assembler instructions
    ****************************************************************************/

    chaptertitle(out, chapter++);
    out <<
        "<table>\n"
        "<thead>\n"
        "<tr>\n"
        "\t<th>group</th>\n"
        "\t<th>syntax</th>\n"
        "\t<th>description</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tbody>\n";

    uint8_t name_length[insn_last];
    uint8_t max_name_length = 0;
    for (int inst = 0; inst < insn_last; inst++) {
        name_length[inst] = static_cast<uint8_t>(strlen(asm_name[inst]));
        max_name_length = std::max(max_name_length, name_length[inst]);
    }
    max_name_length += 2;

    for (int inst = 0; inst < insn_last; inst++) {
        const EInstructionSubset  subset = asm_info[inst].subset;
        const EInstructionHardwareType hard = asm_info[inst].hard;
        out << "<tr>\n"
               "\t<td>" << instr_subset_name[subset] << "</td>\n"
               "\t<td><pre><a href=\"#" << asm_name[inst] << "\">" << asm_name[inst] << "</a>";

        const EInstructionAssemblerFormat format = asm_info[inst].sw_format;
        if (format != asm_fmt_NoArgs)
          out << std::setw(max_name_length - name_length[inst]) << ' ';

        for (const EInstructionField field : asm_fmt_field[format]) {
            if (field == cf_none)
                break;
            out << inst_field_name[field];
        }
        out << "</pre></td>\n"
               "\t<td>" << inst_descr[inst];

        if (hard == instr_htype_sw) {
            out << "*";
        }

        out << "</td>\n"
               "</tr>\n";
    }
    out << "</tbody>\n"
           "</table>\n\n";

    /****************************************************************************
    * machine instruction formats
    ****************************************************************************/

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
        "</tbody></table>\n"

        "<table width=\"60%\">\n"
        "<caption>color descriptions for instruction fields:</caption>\n";

    for (int i = 0; i < inst_fieldtype_last; i++) {
        out << "<tr><td class=\"" << field_type_cssclass[i] << "\">" << field_color_descr[i] << "</td></tr>\n";
    }
    out << "</table>\n"

        "<table width=\"90%\">\n"
        "<caption>slot formats:</caption>\n"
        "<tr>\n"
        "<td class=\"bit\" rowspan=\"2\" style=\"width:14%\">format<br/>name</td>\n"
        "<td class=\"bit\" colspan=\"42\">bit numbers</td>\n"
        "</tr>\n"

    "<tr>\n";
    for (int pos = bits_per_slot - 1; pos >= 0; pos--) {
        out << "<td class=\"bit\">" << pos << "</td>\n";
    }
    out << "</tr>\n";

    for (size_t format = 0; format < sizearray(inst_fmt_field); format++) {
        out << "<tr><td>" << inst_format_name[format] << "</td>\n"
            "\t<td class=\"" << field_type_cssclass[inst_fieldtype_opc]
            << "\" colspan=\"" << static_cast<unsigned>(bits_per_opcode) << "\">opcode</td>\n";

        for (size_t pos=disp_opcode, i=0; pos>0 && i < sizearray(inst_fmt_field[0]); i++) {
            const EInstructionField     field      = inst_fmt_field[format][i];
            const EInstructionSize      field_size = inst_field_data[field].size;
            const EInstructionFieldKind field_type = inst_field_data[field].type;
            if (field_size > 0) {
               pos -= field_size;
               out << "\t<td class=\"" << field_type_cssclass[field_type]
                   << "\" colspan=\"" << static_cast<unsigned>(field_size) << "\">"
                   << inst_field_name[field] << "</td>\n";
               //assert(inst_field_data[field].disp == disp_unknown ||
               //       inst_field_data[field].disp == pos);
            }
        }
        out << "</tr>\n";
    }
    out << "</table>\n";

    /****************************************************************************
    * assembler instructions bit-formatted list
    ****************************************************************************/

    chaptertitle(out, chapter++);
    out << "<table width=\"90%\">\n"
           "<tr>\n"
           "<td class=\"bit\" rowspan=\"2\" style=\"width:14%\">instruction<br/>mnemonic</td>\n"
           "<td class=\"bit\" colspan=\"" << static_cast<unsigned>(bits_per_slot) << "\">bit numbers</td></tr>\n"
           "<tr>\n";

    for (int pos = bits_per_slot - 1; pos >= 0; pos--) {
        out << "<td class=\"bit\">" << pos << "</td>\n";
    }
    out << "</tr>\n";

    for (int inst = 0; inst < insn_last; inst++) {
        uint64_t const cmd = asm_info[inst].tmplt;
        const auto format = asm_info[inst].hw_format;
        const unsigned opcode = CInsn1(cmd).GetOpcode();

        out << "<tr>\n"
               "\t<td><a id=\"" << asm_name[inst] << "\"></a>" << asm_name[inst] << "</td>\n";
        out << "\t<td class=\"" << field_type_cssclass[inst_fieldtype_opc]
            << "\" colspan=\"" << static_cast<unsigned>(bits_per_opcode)
            << "\"><a href=\"#opcode_table\">" << opcode << "</a></td>\n";

        for (int pos = disp_opcode, i = 0; pos > 0; i++) {
            const EInstructionField     field      = inst_fmt_field[format][i];
            const EInstructionSize      field_size = inst_field_data[field].size;
            const EInstructionFieldKind field_type = inst_field_data[field].type;
            if (field_size > 0) {
               pos -= field_size;
               out << "\t<td class=\"" << field_type_cssclass[field_type]
                   << "\" colspan=\"" << static_cast<unsigned>(field_size) << "\">";

               if (field_type == inst_fieldtype_opx) {
                  const unsigned opx = static_cast<unsigned>(get_ubitfield(cmd, pos, field_size));
                  // assert (pos == disp_opx);
                  out << "<a href=\"#" << asm_name[Decoder::arch_info_opcode[opcode]] << "_table\">" << opx << "</a>";
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
    registerfile(out, inst_fieldtype_gpr, decoder.gpr_names, "128-bit general purpose registers");
    registerfile(out, inst_fieldtype_spr, decoder.spr_names, "64-bit special purpose registers");

    /****************************************************************************
    * instruction encoding
    ****************************************************************************/

    chaptertitle(out, chapter++);
    unsigned int opcodes = codetable<1 << bits_per_opcode>(out,
        Decoder::arch_info_opcode, "opcode", "primary opcodes");

    unsigned int opc_name[8], opx_num[8];

    opc_name[0] = insn_fused;
    opx_num[0] = codetable<1 << bits_per_fused_opx>(out, Decoder::arch_info_fused, asm_name[insn_fused],
                "opcode &laquo;fused&raquo; extended opcodes");

    opc_name[1] = insn_raopx;
    // +1 for `undef`, which isn't counted inside codetable but is official instruction
    opx_num[1] = 1 + codetable<1 << bits_per_register>(out, Decoder::arch_info_raopx, asm_name[insn_raopx],
                 "opcode &laquo;raopx&raquo; extended opcodes");

    opc_name[2] = insn_nullify;
    opx_num[2] = codetable<1 << bits_per_nullify_opx>(out, Decoder::arch_info_nullify, asm_name[insn_nullify],
                 "opcode &laquo;nullify&raquo; extended opcodes");

    opc_name[3] = insn_misc;
    opx_num[3] = codetable<1 << bits_per_misc_opx>(out, Decoder::arch_info_misc, asm_name[insn_misc],
                 "opcode &laquo;misc&raquo; extended opcodes");

    const int num_opcodes_with_extcode = 4;

    unsigned int snum[instr_subset_last][instr_htype_last], snum_sum[instr_htype_last];

    for (int hard = 0; hard < instr_htype_last; hard++) {
        snum_sum[hard] = 0;
        for (int subset=0; subset<instr_subset_last; subset++) {
            snum[subset][hard] = 0;
        }
    }

    for (int inst = 0; inst < insn_last; inst++) {
        EInstructionSubset subset = asm_info[inst].subset;
        int hard = asm_info[inst].hard;
        snum[subset][hard]++;
    }

    for (int subset = 0; subset < instr_subset_last; subset++) {
        for (int hard = 0; hard < instr_htype_last; hard++) {
            snum_sum[hard] += snum[subset][hard];
        }
    }
    assert(snum_sum[instr_htype_hw] + snum_sum[instr_htype_sw] == insn_last);

    /*****************************************************************************
    * instruction subset encoding statistic
    *****************************************************************************/

    chaptertitle(out, chapter++);
    out <<
        "<table width=\"60%\">\n"
        "<caption>statistic by instruction subsets:</caption>\n"
        "<thead>\n"
        "<tr>\n"
        "\t<th>instruction subset</th>\n"
        "\t<th>all</th>\n"
        "\t<th>hardwired</th>\n"
        "\t<th>pseudo-ops</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tfoot>\n"
        "<tr>\n"
        "\t<th>sum:</th>\n"
        "\t<th>" << (snum_sum[instr_htype_hw] + snum_sum[instr_htype_sw]) << "</th>\n"
        "\t<th>" << snum_sum[instr_htype_hw] << "</th>\n"
        "\t<th>" << snum_sum[instr_htype_sw] << "</th>\n"
        "</tr>\n"
        "</tfoot>\n"
        "<tbody>\n";

    for (int subset = 0; subset < instr_subset_last; subset++) {
        out << "<tr>\n"
               "\t<td class=\"cnt\">" << instr_subset_name[subset] << "</td>\n"
               "\t<td class=\"cnt\">" << (snum[subset][instr_htype_hw] + snum[subset][instr_htype_sw]) << "</td>\n"
               "\t<td class=\"cnt\">" << snum[subset][instr_htype_hw] << "</td>\n"
               "\t<td class=\"cnt\">" << snum[subset][instr_htype_sw] << "</td>\n"
               "</tr>\n";
    }

    out << "</tbody>\n"
           "</table>\n"
           "<br/>\n";

    /*****************************************************************************
    * extended opcode statistic
    *****************************************************************************/
    int extcodes = 0;
    for (int i = 0; i < num_opcodes_with_extcode; i++) {
        extcodes += opx_num[i];
    }

    const int opc_with_opx_num = insn_reserved - insn_last - 1;

    out <<
        "<table width=\"60%\">\n"
        "<caption>statistic by instruction opcodes (all " << (opcodes - opc_with_opx_num + extcodes) << " codes):</caption>\n"
        "<thead>\n"
        "<tr>\n"
        "\t<th>opcode</th>\n"
        "\t<th>num</th>\n"
        "</tr>\n"
        "</thead>\n"
        "<tbody>\n"
        "<tr>\n"
        "\t<td class=\"cnt\">primary opcodes</td>\n"
        "\t<td class=\"cnt\">" << (opcodes - opc_with_opx_num) << " [+" << opc_with_opx_num << " with extended codes]</td>\n"
        "</tr>\n";

    for (int i = 0; i < num_opcodes_with_extcode; i++) {
        out << "<tr>\n"
               "\t<td class=\"cnt\">" << asm_name[opc_name[i]] << "</td>\n"
               "\t<td class=\"cnt\">" << opx_num[i] << "</td>\n"
               "</tr>\n";
    }
    out << "</tbody>\n"
           "</table>\n";

    out << "<br/>\n"
           "<hr/>\n"
           "<p class=\"copyright\">"
           ARCH_NAME " 64-bit virtual processor<br/>\n"
           "Instruction Set Architecture (ISA) and assembler syntax.\n"
           "<br/>\n"
           ARCH_COPYRIGHT "\n"
           "<br/>\n"
           "This file was generated automatically.\n"
           "</p>\n"
           "</body>\n"
           "</html>\n";

    return 0;
}

} // namespace postrisc
