#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static std::string CodeModelName(CodeModel cm)
{
    switch (cm) {
        case SMALL:  return "small";
        case MEDIUM: return "medium";
        case LARGE:  return "large";
    }
    assert(!"unknown CodeModel");
    return "unknown CM";
}

static std::string ThreadModelName(ThreadModel tm)
{
    switch (tm) {
        case LocalExec: return "LocalExec";
        case LocalDynamic: return "LocalDynamic";
        case GlobalDynamic: return "GlobalDynamic";
        case InitialExec: return "InitialExec";
        case DefaultTLS: return "DefaultTLS";
    }
    assert(!"unknown ThreadModel");
    return "unknown TM";
}

struct addressing_mode {
    EInstructionID load;
    EInstructionID store;
};

struct TypeAddressing {
    addressing_mode base;
    addressing_mode iprel;
    addressing_mode xsd;
    addressing_mode xud;
    addressing_mode xsw;
    addressing_mode xuw;
    addressing_mode mia;
};

static const TypeAddressing ibyte = {
        { insn_ldsb,     insn_stb },
        { insn_ldsbr,    insn_stbr },
        { insn_ldsbxsd,  insn_stbxsd },
        { insn_ldsbxud,  insn_stbxud },
        { insn_ldsbxsw,  insn_stbxsw },
        { insn_ldsbxuw,  insn_stbxuw },
        { insn_ldsbmia,  insn_stbmia },
};

static const TypeAddressing ubyte = {
        { insn_ldub,     insn_stb },
        { insn_ldubr,    insn_stbr },
        { insn_ldubxsd,  insn_stbxsd },
        { insn_ldubxud,  insn_stbxud },
        { insn_ldubxsw,  insn_stbxsw },
        { insn_ldubxuw,  insn_stbxuw },
        { insn_ldubmia,  insn_stbmia },
};

static const TypeAddressing ihalfword = {
        { insn_ldsh,     insn_sth },
        { insn_ldshr,    insn_sthr },
        { insn_ldshxsd,  insn_sthxsd },
        { insn_ldshxud,  insn_sthxud },
        { insn_ldshxsw,  insn_sthxsw },
        { insn_ldshxuw,  insn_sthxuw },
        { insn_ldshmia,  insn_sthmia },
};

static const TypeAddressing uhalfword = {
        { insn_lduh,     insn_sth },
        { insn_lduhr,    insn_sthr },
        { insn_lduhxsd,  insn_sthxsd },
        { insn_lduhxud,  insn_sthxud },
        { insn_lduhxsw,  insn_sthxsw },
        { insn_lduhxuw,  insn_sthxuw },
        { insn_lduhmia,  insn_sthmia },
};

static const TypeAddressing iword = {
        { insn_ldsw,     insn_stw },
        { insn_ldswr,    insn_stwr },
        { insn_ldswxsd,  insn_stwxsd },
        { insn_ldswxud,  insn_stwxud },
        { insn_ldswxsw,  insn_stwxsw },
        { insn_ldswxuw,  insn_stwxuw },
        { insn_ldswmia,  insn_stwmia },
};

static const TypeAddressing uword = {
        { insn_lduw,     insn_stw },
        { insn_lduwr,    insn_stwr },
        { insn_lduwxsd,  insn_stwxsd },
        { insn_lduwxud,  insn_stwxud },
        { insn_lduwxsw,  insn_stwxsw },
        { insn_lduwxuw,  insn_stwxuw },
        { insn_lduwmia,  insn_stwmia },
};

static const TypeAddressing idword = {
        { insn_ldsd,     insn_std },
        { insn_ldsdr,    insn_stdr },
        { insn_ldsdxsd,  insn_stdxsd },
        { insn_ldsdxud,  insn_stdxud },
        { insn_ldsdxsw,  insn_stdxsw },
        { insn_ldsdxuw,  insn_stdxuw },
        { insn_ldsdmia,  insn_stdmia },
};

static const TypeAddressing udword = {
        { insn_ldud,     insn_std },
        { insn_ldudr,    insn_stdr },
        { insn_ldudxsd,  insn_stdxsd },
        { insn_ldudxud,  insn_stdxud },
        { insn_ldudxsw,  insn_stdxsw },
        { insn_ldudxuw,  insn_stdxuw },
        { insn_ldudmia,  insn_stdmia },
};

static const TypeAddressing qword = {
        { insn_ldq,     insn_stq },
        { insn_ldqr,    insn_stqr },
        { insn_ldqxsd,  insn_stqxsd },
        { insn_ldqxud,  insn_stqxud },
        { insn_ldqxsw,  insn_stqxsw },
        { insn_ldqxuw,  insn_stqxuw },
        { insn_ldqmia,  insn_stqmia },
};

static const struct LoadStoreTest {
    std::string create_name(const char *name) const;
    std::string create_name(const char *name, CodeModel cm) const;
    std::string create_name(const char *name, CodeModel cm, ThreadModel tm) const;

    u32 scale() const { return util::ilog2(size); }
    unsigned size;
    ValueType type;
    ValueType memory_type;
    const TypeAddressing ta;
} LoadStoreTests[] = {
    { sizeof(bool),  BOOL, BOOL, ubyte },

    { sizeof(i8),  IB, IB, ibyte },
    { sizeof(i8),  IH, IB, ibyte },
    { sizeof(i8),  IW, IB, ibyte },
    { sizeof(i8),  ID, IB, ibyte },
    { sizeof(i8),  IQ, IB, ibyte },

    { sizeof(u8),  UB, UB, ubyte },
    { sizeof(u8),  UH, UB, ubyte },
    { sizeof(u8),  UW, UB, ubyte },
    { sizeof(u8),  UD, UB, ubyte },
    { sizeof(u8),  UQ, UB, ubyte },

    { sizeof(i16),  IH, IH, ihalfword },
    { sizeof(i16),  IW, IH, ihalfword },
    { sizeof(i16),  ID, IH, ihalfword },
    { sizeof(i16),  IQ, IH, ihalfword },

    { sizeof(u16),  UH, UH, uhalfword },
    { sizeof(u16),  UW, UH, uhalfword },
    { sizeof(u16),  UD, UH, uhalfword },
    { sizeof(u16),  UQ, UH, uhalfword },

    { sizeof(i32),  IW, IW, uword }, // load i32 as u32 (it's ok)
    { sizeof(i32),  ID, IW, iword },
    { sizeof(i32),  IQ, IW, iword },

    { sizeof(u32),  UW, UW, uword },
    { sizeof(u32),  UD, UW, uword },
    { sizeof(u32),  UQ, UW, uword },

    { sizeof(i64),  ID, ID, udword }, // load i64 as u64 (it's ok)
    { sizeof(i64),  IQ, ID, idword },

    { sizeof(u64),  UD, UD, udword },
    { sizeof(u64),  UQ, UD, udword },

    { sizeof(i128), IQ, IQ, qword },
    { sizeof(u128), UQ, UQ, qword },

    { sizeof(native::f16),  FH, FH, uhalfword },
    { sizeof(native::f32),  FS, FS, uword },
    { sizeof(native::f64),  FD, FD, udword },
    { sizeof(native::f128), FQ, FQ, qword },
};

std::string LoadStoreTest::create_name(const char *name) const
{
    std::ostringstream out;
    out << name << "_" << type << "_" << memory_type;
    return out.str();
}

std::string LoadStoreTest::create_name(const char *name, CodeModel cm) const
{
    return create_name(name) + "_" + CodeModelName(cm);
}

std::string LoadStoreTest::create_name(const char *name, CodeModel cm, ThreadModel tm) const
{
    return create_name(name) + "_" + CodeModelName(cm) + "_" + ThreadModelName(tm);
}

static std::string create_name(const char *name, CodeModel cm)
{
    return std::string(name) + "_" + CodeModelName(cm);
}

CODEGEN_HANDLER(LoadBase)
{
    std::string label;
    for (const auto& test : LoadStoreTests) {
        label = test.create_name("load_base");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.base.load)
                          << " %r1, %r1, " << test.size * 400 << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return ptr[400];\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("load_xsd");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsd.load)
                          << " %r1, %r1, %r2, " << test.scale() << ", " << test.size * 2 << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i64 index)\n";
        out << "{\n";
        out << "    return ptr[index+2];\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("load_xsd_ext");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsd.load)
                          << ".l %r1, %r1, %r2, " << test.scale() << ", " << test.size * 1000 << '\n';
        out << CHECK_EMPTY << '\n';
        out << CHECK_NEXT << "! BUNDLE\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i64 index)\n";
        out << "{\n";
        out << "    return ptr[index+1000];\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("load_xsw");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsw.load)
                          << " %r1, %r1, %r2, " << test.scale() << ", " << (test.size * 2) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i32 index)\n";
        out << "{\n";
        out << "    return ptr[index+2];\n";
        out << "}\n";

        label = test.create_name("load_xsw_ext");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsw.load)
                          << ".l %r1, %r1, %r2, " << test.scale() << ", " << (test.size * 1000) << '\n';
        out << CHECK_EMPTY << '\n';
        out << CHECK_NEXT << "! BUNDLE\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i32 index)\n";
        out << "{\n";
        out << "    return ptr[index+1000];\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("lda_xsw");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, insn_ldaxsw)
                          << " %r1, %r1, %r2, " << test.scale() << ", " << (test.size * 2) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.memory_type << " *" << label << "(" << test.memory_type << " *ptr, i32 index)\n";
        out << "{\n";
        out << "    return &ptr[index+2];\n";
        out << "}\n";

        label = test.create_name("load_xuw");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xuw.load)
                          << " %r1, %r1, %r2, " << test.scale() << ", " << 2 << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, u32 index)\n";
        out << "{\n";
        out << "    return ptr[index+2];\n";
        out << "}\n";

        label = test.create_name("load_xuw_ext");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xuw.load)
                          << ".l %r1, %r1, %r2, " << test.scale() << ", " << 1000 << '\n';
        out << CHECK_EMPTY << '\n';
        out << CHECK_NEXT << "! BUNDLE\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, u32 index)\n";
        out << "{\n";
        out << "    return ptr[index+1000];\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("lda_xuw_ext");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, insn_ldaxuw)
                          << ".l %r1, %r1, %r2, " << test.scale() << ", " << 1000 << '\n';
        out << CHECK_EMPTY << '\n';
        out << CHECK_NEXT << "! BUNDLE\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.memory_type << " *" << label << "(" << test.memory_type << " *ptr, u32 index)\n";
        out << "{\n";
        out << "    return &ptr[index+1000];\n";
        out << "}\n";
        out << '\n';
    }
}

CODEGEN_HANDLER(StoreBase)
{
    const int disp = 1;

    for (const auto& test : LoadStoreTests) {
        std::string label = test.create_name("store_base");

        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.base.store) << " %r1, %r2, " << (test.size * 400) << '\n';
        out << CHECK_NEXT << InsnCounter(this, test.ta.base.store) << " %gz, %r2, " << test.size * 100 << '\n';
        out << CHECK_EMPTY << '\n';
        out << CHECK_NEXT << "! BUNDLE\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    ptr[400] = value;\n";
        out << "    ptr[100] = 0;\n";
        out << "}\n";
        out << '\n';

        // store_xsd
        label = test.create_name("store_xsd");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsd.store)
                          << " %r1, %r2, %r3, " << test.scale() << ", " << (test.size * disp) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, i64 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = value;\n";
        out << "}\n";
        out << '\n';

        // store_zero_xsd
        label = test.create_name("store_zero_xsd");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsd.store)
                          << " %gz, %r1, %r2, " << test.scale() << ", " << (test.size * disp) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.memory_type << " *ptr, i64 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = 0;\n";
        out << "}\n";
        out << '\n';

        // store_xud
        label = test.create_name("store_xud");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsd.store)
                          << " %r1, %r2, %r3, " << test.scale() << ", " << (test.size * disp) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, u64 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = value;\n";
        out << "}\n";
        out << '\n';

        // store_zero_xud
        label = test.create_name("store_zero_xud");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsd.store)
                          << " %gz, %r1, %r2, " << test.scale() << ", " << (test.size * disp) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.memory_type << " *ptr, u64 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = 0;\n";
        out << "}\n";
        out << '\n';

        // store_xsw
        label = test.create_name("store_xsw");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsw.store)
                          << " %r1, %r2, %r3, " << test.scale() << ", " << (test.size * disp) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, i32 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = value;\n";
        out << "}\n";
        out << '\n';

        // store_zero_xsw
        label = test.create_name("store_zero_xsw");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xsw.store)
                          << " %gz, %r1, %r2, " << test.scale() << ", " << (test.size * disp) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.memory_type << " *ptr, i32 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = 0;\n";
        out << "}\n";
        out << '\n';

        // store_xuw
        label = test.create_name("store_xuw");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xuw.store) << " %r1, %r2, %r3, " << test.scale() << ", "<< disp  << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, u32 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = value;\n";
        out << "}\n";
        out << '\n';

        // store_zero_xuw
        label = test.create_name("store_zero_xuw");
        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, test.ta.xuw.store) << " %gz, %r1, %r2, " << test.scale() << ", " << disp << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << label << "(" << test.memory_type << " *ptr, u32 index)\n";
        out << "{\n";
        out << "    ptr[index + " << disp << "] = 0;\n";
        out << "}\n";
        out << '\n';
    }
}

void Generator::LoadGlobal(std::ostream& out, CodeModel cm)
{
    const std::string data_suffix = (cm == SMALL) ? "" : ".l";
    const std::string data_reloc = (cm == SMALL) ? "%pcfwd" : "%pcfwd_ext";

    const std::string code_suffix = (cm != LARGE) ? "" : ".l";
    const std::string code_reloc = (cm != LARGE) ? "%jpc" : "%jpc_ext";

    {
        const std::string func = create_name("global_func", cm);
        out << "extern int " << func << "(int arg);\n";

        const std::string func_ptr = create_name("global_func_ptr", cm);
        out << "extern int (*" << func_ptr << ")(int arg);\n";

        out << "// function pointer materialize: " << insn_ldarc << '\n';

        std::string label = create_name(to_string(insn_ldarc), cm);

        out << CHECK_LABEL << label << '\n';
        out << CHECK << InsnCounter(this, insn_ldarc) << code_suffix
            << " %r1, " << code_reloc << "(" << func << ")\n";
        out << "ft " << label << "(void)\n";
        out << "{\n";
        out << "    return &"  << func << ";\n";
        out << "}\n";
        out << '\n';

        out << "// function call: " << insn_call << '\n';

        label = create_name(to_string(insn_call), cm);

        out << CHECK_LABEL << label << '\n';
        out << CHECK << InsnCounter(this, insn_call) << code_suffix
            << " %r2, " << code_reloc << "(" << func << ")\n";
        out << "int " << label << "(int value)\n";
        out << "{\n";
        out << "    return "  << func << "(value);\n";
        out << "}\n";
        out << '\n';

        out << "// function ptr call: " << insn_callplt << '\n';

        label = create_name(to_string(insn_callplt), cm);

        out << CHECK_LABEL << label << '\n';
        out << CHECK << InsnCounter(this, insn_callplt) << data_suffix
            << " %r2, " << data_reloc << "(" << func_ptr << ")\n";
        out << "int " << label << "(int value)\n";
        out << "{\n";
        out << "    return "  << func_ptr << "(value);\n";
        out << "}\n";
        out << '\n';

        label = create_name("callplt_folded", cm);

        out << "// test folding immediate into callplt\n";

        const std::string func_ptr_table = create_name("global_func_ptr_table", cm);
        out << "extern ft " << func_ptr_table << "[20];\n";

        out << CHECK_LABEL << label << '\n';
        out << CHECK << insn_callplt << data_suffix
            << " %r2, " << data_reloc << "(" << func_ptr_table << "+" << (sizeof(void*) * 5) << ")\n";
        out << "int "  << label << "(int arg)\n";
        out << "{\n";
        out << "    return "  << func_ptr_table << "[5](arg);\n";
        out << "}\n";
        out << '\n';
    }

    for (const auto& test : LoadStoreTests) {
        const std::string object = test.create_name("global_", cm);
        out << "extern " << test.memory_type << " " << object << ";\n";

        const std::string object_array = test.create_name("global_array_", cm);
        out << "extern " << test.memory_type << " " << object_array << "[1000];\n";

        std::string label = test.create_name("ldard", cm);

        out << CHECK_LABEL << label << '\n';
        out << CHECK << InsnCounter(this, insn_ldard) << data_suffix
            << " %r1, " << data_reloc << "(" << object << ")\n";
        out << test.memory_type << "* " << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return &"  << object << ";\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("ldard_folded", cm);

        out << "// test folding immediate into lda iprel\n";
        out << CHECK_LABEL << label << '\n';
        out << CHECK << InsnCounter(this, insn_ldard) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array << "+" << (test.size * 100) << ")\n";
        out << test.memory_type << "* "  << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return &"  << object_array << "[100];\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("load_iprel", cm);

        out << CHECK_LABEL << label << '\n';
        out << CHECK << InsnCounter(this, test.ta.iprel.load) << data_suffix
            << " %r1, " << data_reloc << "("  << object << ")\n";
        out << test.type << " "  << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return "  << object << ";\n";
        out << "}\n";
        out << '\n';

        label = test.create_name("load_iprel_folded", cm);

        out << "// test folding immediate info ld_iprel\n";
        out << CHECK_LABEL << label << '\n';
        out << CHECK << InsnCounter(this, test.ta.iprel.load) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array << "+" << (test.size * 100) << ")\n";
        out << test.type << " " << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return " << object_array << "[100];\n";
        out << "}\n";
        out << '\n';
    }
}

CODEGEN_HANDLER(LoadGlobalSmall)  { LoadGlobal(out, SMALL); }
CODEGEN_HANDLER(LoadGlobalMedium) { LoadGlobal(out, MEDIUM); }
CODEGEN_HANDLER(LoadGlobalLarge)  { LoadGlobal(out, LARGE); }

void Generator::StoreGlobal(std::ostream& out, CodeModel cm)
{
    const std::string data_suffix = (cm == SMALL) ? "" : ".l";
    const std::string data_reloc = (cm == SMALL) ? "%pcfwd" : "%pcfwd_ext";

    for (const auto& test : LoadStoreTests) {
        const std::string object = test.create_name("global_", cm);
        out << "extern " << test.memory_type << " " << object << ";\n";

        const std::string object2 = test.create_name("global2_", cm);
        out << "extern " << test.memory_type << " " << object2 << ";\n";

        const std::string object_array = test.create_name("global_array_", cm);
        out << "extern " << test.memory_type << " " << object_array << "[1000];\n";

        std::string label = test.create_name("store_iprel", cm);

        out << CHECK_LABEL << label << '\n';
        out << "void " << label << "(" << test.type << " value)\n";
        out << "{\n";

        out << CHECK << InsnCounter(this, test.ta.iprel.store) << data_suffix
            << " %r1, " << data_reloc << "(" << object << ")\n";
        out << "    " << object << " = value;\n";

        out << CHECK << InsnCounter(this, test.ta.iprel.store) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array << "+" << (test.size * 100) << ")\n";
        out << "    " << object_array << "[100] = value;\n";

        out << "// test store-zero\n";
        out << CHECK << InsnCounter(this, test.ta.iprel.store) << data_suffix
            << " %gz, " << data_reloc << "(" << object2 << ")\n";
        out << "    " << object2 << " = 0;\n";

        out << "// test store-zero\n";
        out << CHECK << InsnCounter(this, test.ta.iprel.store) << data_suffix
            << " %gz, " << data_reloc << "(" << object_array << "+" << (test.size * 200) << ")\n";
        out << "    " << object_array << "[200] = 0;\n";

        out << "}\n";
        out << '\n';
    }
}

CODEGEN_HANDLER(StoreGlobalSmall)  { StoreGlobal(out, SMALL); }
CODEGEN_HANDLER(StoreGlobalMedium) { StoreGlobal(out, MEDIUM); }
CODEGEN_HANDLER(StoreGlobalLarge)  { StoreGlobal(out, LARGE); }

/*
global-dynamic:
    address = __tls_get_addr(ip + tgd(x))) // R_POSTRISC_TLS_GD_GOT_OFFSET

local-dynamic:
    tls_block = __tls_get_addr(ip + tld(x1 or x2)) // R_POSTRISC_TLS_GD_GOT_OFFSET
    address_x1 = tls_block + dtpoff(x1) // R_POSTRISC_TLS_LD_OFFSET
    address_x2 = tls_block + dtpoff(x2) // R_POSTRISC_TLS_LD_OFFSET
    (may be folded with load/store)

initial-exec:
    address = TP + load(ip + gottpoff(x)) // R_POSTRISC_TLS_IE_OFFSET

local-exec:
    address = TP + tpoff(x) // R_POSTRISC_TLS_LE_OFFSET
    (may be folded with load/store)
*/

// FIXME: __tls_get_addr will be long?

void Generator::LoadStoreTLS(std::ostream& out, CodeModel cm, ThreadModel tm)
{
    const std::string data_suffix = (cm == SMALL) ? "" : ".l";
    const std::string data_reloc = (cm == SMALL) ? "%pcfwd" : "%pcfwd_ext";

    const std::string code_suffix = (cm != LARGE) ? "" : ".l";
    const std::string code_reloc = (cm != LARGE) ? "%jpc" : "%jpc_ext";

    const std::string tpoff = (cm == SMALL) ? "%tpoff" : "%tpoff_ext";
    const std::string dtpoff = (cm == SMALL) ? "%dtpoff" : "%dtpoff_ext";
    const std::string gottpoff = (cm == SMALL) ? "%gottpoff" : "%gottpoff_ext";
    const std::string tld = (cm == SMALL) ? "%tld": "%tld_ext";
    const std::string tgd = (cm == SMALL) ? "%tgd": "%tgd_ext";

    for (const auto& test : LoadStoreTests) {
        // __thread i64 tls_small_i64;
        const std::string object = test.create_name("tls", cm, tm);
        out << "__thread " << test.memory_type << " " << object << ";\n";

        // __thread i64 tls_array_small_i64[100];
        const std::string object_array = test.create_name("tls_array", cm, tm);
        out << "__thread " << test.memory_type << " " << object_array << "[1000];\n";

        //----------------------------------------------------------------------------
        // load TLS address
        //----------------------------------------------------------------------------
        std::string label = test.create_name("lda_tls", cm, tm);
        out << CHECK_LABEL << label << '\n';

        switch (tm) {
            case LocalExec:
                // addid %r1, %tp, %tpoff(tls_small_LocalExec_bool)
                out << CHECK << insn_addid << data_suffix << " %r1, %tp, " << tpoff << "(" << object << ")\n";
                break;
            case LocalDynamic:
                // ldard %r3, %tld(tls_small_LocalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // addid %r1, %r3, %dtpoff(tls_small_LocalDynamic_bool)
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tld << "(" << object << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << insn_addid << data_suffix << " %r1, %r3, " << dtpoff << "(" << object << ")\n";
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // ldard %r3, %tgd(tls_small_GlobalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // mov %r1, %r3
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tgd << "(" << object << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << insn_mov << " %r1, %r3\n";
                break;
            case InitialExec:
                // ldudr.l %r1, %gottpoff_ext(tls_large_InitialExec_i8)
                // addd %r1, %tp, %r1
                out << CHECK << insn_ldudr << data_suffix << " %r1, " << gottpoff << "(" << object << ")\n";
                out << CHECK << insn_addd << " %r1, %tp, %r1\n";
                break;
        }

        out << test.memory_type << "* " << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return &"  << object << ";\n";
        out << "}\n";
        out << '\n';

        //----------------------------------------------------------------------------
        // load TLS address (folded)
        //----------------------------------------------------------------------------
        label = test.create_name("lda_tls_folded", cm, tm);
        out << "// test TLS array folding (load address)\n";
        out << CHECK_LABEL << label << '\n';

        switch (tm) {
            case LocalExec:
                // ldi.l %r1, %tpoff_ext(tls_array_medium_LocalExec_bool)
                // ldaxsd.l %r1, %tp, %r1, 0, 100
                out << CHECK << insn_ldi << data_suffix << " %r1, " << tpoff << "("  << object_array << ")\n";
                out << CHECK << insn_ldaxsd << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << '\n';
                break;
            case LocalDynamic:
                // ldard %r3, %tld(tls_array_small_LocalDynamic)
                // call %r2, %jpc(__tls_get_addr)
                // ldi %r1, %dtpoff(tls_array_small_LocalDynamic)
                // ldaxsd.l %r1, %r3, %r1, 0, 100
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tld << "(" << object_array << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << insn_ldi << data_suffix << " %r1, " << dtpoff << "("  << object_array << ")\n";
                out << CHECK << insn_ldaxsd << ".l %r1, %r3, %r1, 0, " << (100 * test.size) << '\n';
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // ldard %r3, %tgd(tls_array_small_GlobalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // addid %r1, %r3, 100
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tgd << "(" << object_array << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << insn_addid << " %r1, %r3, " << (100 * test.size) << '\n';
                break;
            case InitialExec:
                // ldudr.l %r1, %gottpoff_ext(tls_array_large_InitialExec_bool) 
                // ldaxsd.l %r1, %tp, %r1, 0, 100 
                out << CHECK << InsnCounter(this, insn_ldudr) << data_suffix << " %r1, " << gottpoff << "(" << object_array << ")\n";
                out << CHECK << InsnCounter(this, insn_ldaxsd) << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << '\n';
                break;
        }

        out << test.memory_type << "* "  << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return &"  << object_array << "[100];\n";
        out << "}\n";
        out << '\n';

        //----------------------------------------------------------------------------
        // load TLS variable
        //----------------------------------------------------------------------------
        label = test.create_name("load_tls", cm, tm);
        out << CHECK_LABEL << label << '\n';

        switch (tm) {
            case LocalExec:
                // ldud.l %r2, %tp, %tpoff_ext(x_tls_extern)
                out << CHECK << test.ta.base.load << data_suffix << " %r1, %tp, " << tpoff << "("  << object << ")\n";
                break;
            case LocalDynamic:
                // ldard.l %r3, %tld_ext(x_tls_extern)
                // call %r2, %jpc(__tls_get_addr)
                // ldud.l %r1, %r3, %dtpoff_ext(x_tls_extern)
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tld << "("  << object << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << test.ta.base.load << data_suffix << " %r1, %r3, " << dtpoff << "("  << object << ")\n";
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // ldard.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // ldud  %r1, %r3, 0
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tgd << "("  << object << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << test.ta.base.load << " %r1, %r3, 0\n";
                break;
            case InitialExec:
                // ldudr.l %r1, %gottpoff_ext(x_tls_extern)
                // ldudxsd %r1, %tp, %r1, 0, 0
                out << CHECK << insn_ldudr << data_suffix << " %r1, " << gottpoff << "("  << object << ")\n";
                out << CHECK << test.ta.xsd.load << " %r1, %tp, %r1, 0, 0\n";
                break;
        }

        out << test.type << " "  << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return "  << object << ";\n";
        out << "}\n";
        out << '\n';

        //----------------------------------------------------------------------------
        // load TLS variable (folded - array[100])
        //----------------------------------------------------------------------------
        out << "// test TLS array folding (load)\n";
        label = test.create_name("load_tls_folded", cm, tm);
        out << CHECK_LABEL << label << '\n';

        switch (tm) {
            case LocalExec:
                // FIXME(?): ldud.l %r2, %tp, %tpoff_ext(x_tls_extern)
                // ldi %r1, %tpoff(tls_array_small_LocalExec_i8)
                // ldsbxsd.l %r1, %tp, %r1, 0, 100
                out << CHECK << insn_ldi << data_suffix << " %r1, " << tpoff << "("  << object_array << ")\n";
                out << CHECK << test.ta.xsd.load << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << '\n';
                break;
            case LocalDynamic:
                // FIXME(?): ldard.l %r3, %tld_ext(x_tls_extern)
                // FIXME(?): call %r2, %jpc(__tls_get_addr)
                // FIXME(?): ld_u64.l %r1, %r3, %dtpoff_ext(x_tls_extern)

                // ldard.l %r3, %tld_ext(tls_array_large_LocalDynamic_i8) 
                // call.l %r2, %jpc_ext(__tls_get_addr) 
                // ldi.l %r1, %dtpoff_ext(tls_array_large_LocalDynamic_i8) 
                // ldsbxsd.l %r1, %r3, %r1, 0, 100 

                out << CHECK << insn_ldard << data_suffix << " %r3, " << tld << "("  << object_array << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << insn_ldi << data_suffix << " %r1, " << dtpoff << "("  << object_array << ")\n";
                out << CHECK << InsnCounter(this, test.ta.xsd.load) << ".l %r1, %r3, %r1, 0, " << (100 * test.size) << '\n';
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // ldard.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // ldud %r1, %r3, 0
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tgd << "("  << object_array << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << InsnCounter(this, test.ta.base.load) << " %r1, %r3, " << (100 * test.size) << '\n';
                break;
            case InitialExec:
                // ldudr.l %r1, %gottpoff_ext(x_tls_extern)
                // ldudxsd %r1, %tp, %r1, 0, 0
                out << CHECK << insn_ldudr << data_suffix << " %r1, " << gottpoff << "("  << object_array << ")\n";
                out << CHECK << InsnCounter(this, test.ta.xsd.load) << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << '\n';
                break;
        }

        out << test.type << " " << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";
        out << "    return " << object_array << "[100];\n";
        out << "}\n";
        out << '\n';

        //----------------------------------------------------------------------------
        // store TLS variable
        //----------------------------------------------------------------------------
        label = test.create_name("store_tls", cm, tm);
        out << CHECK_LABEL << label << '\n';

        switch (tm) {
            case LocalExec:
                // std.l %r1, %tp, %tpoff_ext(x_tls_extern)
                out << CHECK << test.ta.base.store << data_suffix << " %r1, %tp, " << tpoff << "("  << object << ")\n";
                break;
            case LocalDynamic:
                // ldard.l %r3, %tld_ext(x_tls_extern)
                // call %r2, %jpc(__tls_get_addr)
                // std.l %r1, %r3, %dtpoff_ext(x_tls_extern)
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tld << "("  << object << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << InsnCounter(this, test.ta.base.store) << data_suffix << " %r1, %r3, " << dtpoff << "("  << object << ")\n";
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // ldard.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // std %r1, %r3, 0
                out << CHECK << insn_ldard << data_suffix << " %r3, " << tgd << "("  << object << ")\n";
                out << CHECK << insn_call << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)\n";
                out << CHECK << InsnCounter(this, test.ta.base.store) << " %r1, %r3, 0\n";
                break;
            case InitialExec:
                // ldudr.l %r2, %gottpoff_ext(x_tls_extern)
                // stdxsd %r1, %tp, %r2, 0, 0
                out << CHECK << insn_ldudr << data_suffix << " %r2, " << gottpoff << "("  << object << ")\n";
                out << CHECK << InsnCounter(this, test.ta.xsd.store) << " %r1, %tp, %r2, 0, 0\n";
                break;
        }

        out << "void "  << label << "(" << test.type << " value)\n";
        out << "{\n";
        out << "    "  << object << " = value;\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(LoadStoreTlsSmallLocalExec) { LoadStoreTLS(out, SMALL, LocalExec); }
CODEGEN_HANDLER(LoadStoreTlsSmallLocalDynamic) { LoadStoreTLS(out, SMALL, LocalDynamic); }
CODEGEN_HANDLER(LoadStoreTlsSmallGlobalDynamic) { LoadStoreTLS(out, SMALL, GlobalDynamic); }
CODEGEN_HANDLER(LoadStoreTlsSmallInitialExec) { LoadStoreTLS(out, SMALL, InitialExec); }
CODEGEN_HANDLER(LoadStoreTlsSmallDefault) { LoadStoreTLS(out, SMALL, DefaultTLS); }

CODEGEN_HANDLER(LoadStoreTlsMediumLocalExec) { LoadStoreTLS(out, MEDIUM, LocalExec); }
CODEGEN_HANDLER(LoadStoreTlsMediumLocalDynamic) { LoadStoreTLS(out, MEDIUM, LocalDynamic); }
CODEGEN_HANDLER(LoadStoreTlsMediumGlobalDynamic) { LoadStoreTLS(out, MEDIUM, GlobalDynamic); }
CODEGEN_HANDLER(LoadStoreTlsMediumInitialExec) { LoadStoreTLS(out, MEDIUM, InitialExec); }
CODEGEN_HANDLER(LoadStoreTlsMediumDefault) { LoadStoreTLS(out, MEDIUM, DefaultTLS); }

CODEGEN_HANDLER(LoadStoreTlsLargeLocalExec) { LoadStoreTLS(out, LARGE, LocalExec); }
CODEGEN_HANDLER(LoadStoreTlsLargeLocalDynamic) { LoadStoreTLS(out, LARGE, LocalDynamic); }
CODEGEN_HANDLER(LoadStoreTlsLargeGlobalDynamic) { LoadStoreTLS(out, LARGE, GlobalDynamic); }
CODEGEN_HANDLER(LoadStoreTlsLargeInitialExec) { LoadStoreTLS(out, LARGE, InitialExec); }
CODEGEN_HANDLER(LoadStoreTlsLargeDefault) { LoadStoreTLS(out, LARGE, DefaultTLS); }

CODEGEN_HANDLER(LoadStorePostInc)
{
    std::string label;
    for (const auto& test : LoadStoreTests) {
        label = test.create_name("load_post_inc");
        out << CHECK_LABEL << label << '\n';

        out << "extern int " << label << "_consumer(" << test.type << ");\n";

        out << "int " << label << "(" << test.memory_type << " *ptr)\n";
        out << "{\n";

        if (test.memory_type == BOOL && test.type == BOOL)
            out << CHECK << insn_alloc << " 6\n";
        else
            out << CHECK << insn_alloc << " 5\n";

        out << CHECK << insn_ldi << " %r1, -100\n";
        out << "    int sum = -100;\n";
        out << "    while (sum < 0) {\n";

        if (test.memory_type == BOOL && test.type == BOOL)
            out << CHECK << InsnCounter(this, test.ta.mia.load) << " %r3, %r2, " << (test.size * 2) << '\n';
        else
            out << CHECK << InsnCounter(this, test.ta.mia.load) << " %r4, %r2, " << (test.size * 2) << '\n';

        out << "        sum += " << label << "_consumer(*ptr); ptr += 2;\n";
        out << "    }\n";
        out << CHECK << insn_retf << " 0\n";
        out << "    return sum;\n";
        out << "}\n";
        out << '\n';
    }

    for (const auto& test : LoadStoreTests) {
        label = test.create_name("store_post_inc");
        out << CHECK_LABEL << label << '\n';

        out << "void " << label << "(" << test.memory_type << " *ptr, " << test.type << " value)\n";
        out << "{\n";
        out << CHECK << insn_alloc << " 5\n";
        out << "    for (int i = 0; i < 100; i++) {\n";
        out << CHECK << InsnCounter(this, test.ta.mia.store) << " %r2, %r1, " << (test.size * 2) << '\n';
        out << "        *ptr = value; ptr += 2;\n";
        out << "    }\n";
        out << CHECK << insn_retf << " 0\n";
        out << "}\n";
        out << '\n';
    }
}

} // namespace llvm
} // namespace postrisc
