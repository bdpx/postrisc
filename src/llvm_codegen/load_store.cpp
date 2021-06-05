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
}

struct addressing_mode {
    EInstructionID load;
    EInstructionID store;
};

static const struct LoadStoreTest {
    std::string create_name(const char *name) const;
    std::string create_name(const char *name, CodeModel cm) const;
    std::string create_name(const char *name, CodeModel cm, ThreadModel tm) const;
    unsigned size;
    unsigned scale;
    char type[8];
    char memory_type[8];
    addressing_mode base;
    addressing_mode iprel;
    addressing_mode xi64;
    addressing_mode xu64;
    addressing_mode xi32;
    addressing_mode xu32;
} LoadStoreTests[] = {
#define XXX(size, scale, type, memory_type, load_type, store_type) \
    { size, scale, #type, #memory_type, \
        { insn_ld_       ## load_type, insn_st_       ## store_type }, \
        { insn_ld_iprel_ ## load_type, insn_st_iprel_ ## store_type }, \
        { insn_ld_xi64_  ## load_type, insn_st_xi64_  ## store_type }, \
        { insn_ld_xu64_  ## load_type, insn_st_xu64_  ## store_type }, \
        { insn_ld_xi32_  ## load_type, insn_st_xi32_  ## store_type }, \
        { insn_ld_xu32_  ## load_type, insn_st_xu32_  ## store_type }, \
    },

    XXX(sizeof(bool),         0, bool, bool, u8,   i8)

    XXX(sizeof(i8),           0, i8,   i8,   i8,   i8)
    XXX(sizeof(i8),           0, i16,  i8,   i8,   i8)
    XXX(sizeof(i8),           0, i32,  i8,   i8,   i8)
    XXX(sizeof(i8),           0, i64,  i8,   i8,   i8)
    XXX(sizeof(i8),           0, i128, i8,   i8,   i8)

    XXX(sizeof(u8),           0, u8,   u8,   u8,   i8)
    XXX(sizeof(u8),           0, u16,  u8,   u8,   i8)
    XXX(sizeof(u8),           0, u32,  u8,   u8,   i8)
    XXX(sizeof(u8),           0, u64,  u8,   u8,   i8)
    XXX(sizeof(u8),           0, u128, u8,   u8,   i8)

    XXX(sizeof(i16),          1, i16,  i16,  i16,  i16)
    XXX(sizeof(i16),          1, i32,  i16,  i16,  i16)
    XXX(sizeof(i16),          1, i64,  i16,  i16,  i16)
    XXX(sizeof(i16),          1, i128, i16,  i16,  i16)

    XXX(sizeof(u16),          1, u16,  u16,  u16,  i16)
    XXX(sizeof(u16),          1, u32,  u16,  u16,  i16)
    XXX(sizeof(u16),          1, u64,  u16,  u16,  i16)
    XXX(sizeof(u16),          1, u128, u16,  u16,  i16)

    XXX(sizeof(i32),          2, i32,  i32,  u32,  i32) // load i32 as i32 is insn_XXXX_u32 (it's ok)
    XXX(sizeof(i32),          2, i64,  i32,  i32,  i32)
    XXX(sizeof(i32),          2, i128, i32,  i32,  i32)

    XXX(sizeof(u32),          2, u32,  u32,  u32,  i32)
    XXX(sizeof(u32),          2, u64,  u32,  u32,  i32)
    XXX(sizeof(u32),          2, u128, u32,  u32,  i32)

    XXX(sizeof(i64),          3, i64,  i64,  u64,  i64) // load i64 as i64 is insn_XXXX_u64 (it's ok)
    XXX(sizeof(i64),          3, i128, i64,  i64,  i64)

    XXX(sizeof(u64),          3, u64,  u64,  u64,  i64)
    XXX(sizeof(u64),          3, u128, u64,  u64,  i64)

    XXX(sizeof(i128),         4, i128, i128, i128, i128)
    XXX(sizeof(u128),         4, u128, u128, i128, i128)

    XXX(sizeof(native::f16),  1, f16,  f16,  u16,  i16)
    XXX(sizeof(native::f32),  2, f32,  f32,  u32,  i32)
    XXX(sizeof(native::f64),  3, f64,  f64,  u64,  i64)
    XXX(sizeof(native::f128), 4, f128, f128, i128, i128)
#undef XXX
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
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.base.load)
                          << " %r1, %r1, " << test.size * 400 << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[400];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("load_xi64");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi64.load)
                          << " %r1, %r1, %r2, " << test.scale << ", " << test.size * 2 << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+2];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("load_xi64_ext");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi64.load)
                          << ".l %r1, %r1, %r2, " << test.scale << ", " << test.size * 1000 << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("load_xi32");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi32.load)
                          << " %r1, %r1, %r2, " << test.scale << ", " << (test.size * 2) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+2];" << std::endl;
        out << "}" << std::endl;

        label = test.create_name("load_xi32_ext");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi32.load)
                          << ".l %r1, %r1, %r2, " << test.scale << ", " << (test.size * 1000) << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("lda_xi32");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, insn_lda_xi32)
                          << " %r1, %r1, %r2, " << test.scale << ", " << (test.size * 2) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.memory_type << " *" << label << "(" << test.memory_type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return &ptr[index+2];" << std::endl;
        out << "}" << std::endl;

        label = test.create_name("load_xu32");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xu32.load)
                          << " %r1, %r1, %r2, " << test.scale << ", " << 2 << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, u32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+2];" << std::endl;
        out << "}" << std::endl;

        label = test.create_name("load_xu32_ext");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xu32.load)
                          << ".l %r1, %r1, %r2, " << test.scale << ", " << 1000 << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << label << "(" << test.memory_type << " *ptr, u32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("lda_xu32_ext");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, insn_lda_xu32)
                          << ".l %r1, %r1, %r2, " << test.scale << ", " << 1000 << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << test.memory_type << " *" << label << "(" << test.memory_type << " *ptr, u32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return &ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(StoreBase)
{
    const int disp = 1;

    for (const auto& test : LoadStoreTests) {
        std::string label = test.create_name("store_base");

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.base.store) << " %r1, %r2, " << (test.size * 400) << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.base.store) << " %gz, %r2, " << test.size * 100 << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[400] = value;" << std::endl;
        out << "    ptr[100] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_xi64
        label = test.create_name("store_xi64");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi64.store)
                          << " %r1, %r2, %r3, " << test.scale << ", " << (test.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_zero_xi64
        label = test.create_name("store_zero_xi64");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi64.store)
                          << " %gz, %r1, %r2, " << test.scale << ", " << (test.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.memory_type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_xu64
        label = test.create_name("store_xu64");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi64.store)
                          << " %r1, %r2, %r3, " << test.scale << ", " << (test.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, u64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_zero_xu64
        label = test.create_name("store_zero_xu64");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi64.store)
                          << " %gz, %r1, %r2, " << test.scale << ", " << (test.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.memory_type << " *ptr, u64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_xi32
        label = test.create_name("store_xi32");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi32.store)
                          << " %r1, %r2, %r3, " << test.scale << ", " << (test.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_zero_xi32
        label = test.create_name("store_zero_xi32");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xi32.store)
                          << " %gz, %r1, %r2, " << test.scale << ", " << (test.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.memory_type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_xu32
        label = test.create_name("store_xu32");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xu32.store) << " %r1, %r2, %r3, " << test.scale << ", "<< disp  << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.type << " value, " << test.memory_type << " *ptr, u32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_zero_xu32
        label = test.create_name("store_zero_xu32");
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.xu32.store) << " %gz, %r1, %r2, " << test.scale << ", " << disp << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << test.memory_type << " *ptr, u32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
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
        out << "extern int " << func << "(int arg);" << std::endl;

        const std::string func_ptr = create_name("global_func_ptr", cm);
        out << "extern int (*" << func_ptr << ")(int arg);" << std::endl;

        out << "// function pointer materialize: " << insn_name(insn_lda_r) << std::endl;

        std::string label = create_name(insn_name(insn_lda_r), cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << InsnCounter(this, insn_lda_r) << code_suffix
            << " %r1, " << code_reloc << "(" << func << ")" << std::endl;
        out << "ft " << label << "(void)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << func << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// function call: " << insn_name(insn_call) << std::endl;

        label = create_name(insn_name(insn_call), cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << InsnCounter(this, insn_call) << code_suffix
            << " %r2, " << code_reloc << "(" << func << ")" << std::endl;
        out << "int " << label << "(int value)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << func << "(value);" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// function ptr call: " << insn_name(insn_call_plt) << std::endl;

        label = create_name(insn_name(insn_call_plt), cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << InsnCounter(this, insn_call_plt) << data_suffix
            << " %r2, " << data_reloc << "(" << func_ptr << ")" << std::endl;
        out << "int " << label << "(int value)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << func_ptr << "(value);" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = create_name("call_plt_folded", cm);

        out << "// test folding immediate into call_plt\n";

        const std::string func_ptr_table = create_name("global_func_ptr_table", cm);
        out << "extern ft " << func_ptr_table << "[20];" << std::endl;

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_call_plt) << data_suffix
            << " %r2, " << data_reloc << "(" << func_ptr_table << "+" << (sizeof(void*) * 5) << ")" << std::endl;
        out << "int "  << label << "(int arg)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << func_ptr_table << "[5](arg);" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }

    for (const auto& test : LoadStoreTests) {
        const std::string object = test.create_name("global_", cm);
        out << "extern " << test.memory_type << " " << object << ";" << std::endl;

        const std::string object_array = test.create_name("global_array_", cm);
        out << "extern " << test.memory_type << " " << object_array << "[1000];" << std::endl;

        std::string label = test.create_name("lda_iprel", cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << InsnCounter(this, insn_lda_iprel) << data_suffix
            << " %r1, " << data_reloc << "(" << object << ")" << std::endl;
        out << test.memory_type << "* " << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("lda_iprel_folded", cm);

        out << "// test folding immediate into lda iprel\n";
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << InsnCounter(this, insn_lda_iprel) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array << "+" << (test.size * 100) << ")" << std::endl;
        out << test.memory_type << "* "  << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object_array << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("load_iprel", cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << InsnCounter(this, test.iprel.load) << data_suffix
            << " %r1, " << data_reloc << "("  << object << ")" << std::endl;
        out << test.type << " "  << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << object << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = test.create_name("load_iprel_folded", cm);

        out << "// test folding immediate info ld_iprel\n";
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << InsnCounter(this, test.iprel.load) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array << "+" << (test.size * 100) << ")" << std::endl;
        out << test.type << " " << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << object_array << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
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
        out << "extern " << test.memory_type << " " << object << ";" << std::endl;

        const std::string object2 = test.create_name("global2_", cm);
        out << "extern " << test.memory_type << " " << object2 << ";" << std::endl;

        const std::string object_array = test.create_name("global_array_", cm);
        out << "extern " << test.memory_type << " " << object_array << "[1000];" << std::endl;

        std::string label = test.create_name("store_iprel", cm);

        out << CHECK_LABEL << label << std::endl;
        out << "void " << label << "(" << test.type << " value)" << std::endl;
        out << "{" << std::endl;

        out << CHECK << InsnCounter(this, test.iprel.store) << data_suffix
            << " %r1, " << data_reloc << "(" << object << ")" << std::endl;
        out << "    " << object << " = value;" << std::endl;

        out << CHECK << InsnCounter(this, test.iprel.store) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array << "+" << (test.size * 100) << ")" << std::endl;
        out << "    " << object_array << "[100] = value;" << std::endl;

        out << "// test store-zero" << std::endl;
        out << CHECK << InsnCounter(this, test.iprel.store) << data_suffix
            << " %gz, " << data_reloc << "(" << object2 << ")" << std::endl;
        out << "    " << object2 << " = 0;" << std::endl;

        out << "// test store-zero" << std::endl;
        out << CHECK << InsnCounter(this, test.iprel.store) << data_suffix
            << " %gz, " << data_reloc << "(" << object_array << "+" << (test.size * 200) << ")" << std::endl;
        out << "    " << object_array << "[200] = 0;" << std::endl;

        out << "}" << std::endl;
        out << std::endl;
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
        out << "__thread " << test.memory_type << " " << object << ";" << std::endl;

        // __thread i64 tls_array_small_i64[100];
        const std::string object_array = test.create_name("tls_array", cm, tm);
        out << "__thread " << test.memory_type << " " << object_array << "[1000];" << std::endl;

        //----------------------------------------------------------------------------
        // load TLS address
        //----------------------------------------------------------------------------
        std::string label = test.create_name("lda_tls", cm, tm);
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // add_imm_i64 %r1, %tp, %tpoff(tls_small_LocalExec_bool)
                out << CHECK << insn_name(insn_add_imm_i64) << data_suffix << " %r1, %tp, " << tpoff << "(" << object << ")" << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel %r3, %tld(tls_small_LocalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // add_imm_i64 %r1, %r3, %dtpoff(tls_small_LocalDynamic_bool)
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "(" << object << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_add_imm_i64) << data_suffix << " %r1, %r3, " << dtpoff << "(" << object << ")" << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel %r3, %tgd(tls_small_GlobalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // mov %r1, %r3
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "(" << object << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_mov) << " %r1, %r3" << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(tls_large_InitialExec_i8)
                // add_i64 %r1, %tp, %r1
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "(" << object << ")" << std::endl;
                out << CHECK << insn_name(insn_add_i64) << " %r1, %tp, %r1" << std::endl;
                break;
        }

        out << test.memory_type << "* " << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // load TLS address (folded)
        //----------------------------------------------------------------------------
        label = test.create_name("lda_tls_folded", cm, tm);
        out << "// test TLS array folding (load address)\n";
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // ld_imm.l %r1, %tpoff_ext(tls_array_medium_LocalExec_bool)
                // lda_xi64.l %r1, %tp, %r1, 0, 100
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << tpoff << "("  << object_array << ")" << std::endl;
                out << CHECK << insn_name(insn_lda_xi64) << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel %r3, %tld(tls_array_small_LocalDynamic)
                // call %r2, %jpc(__tls_get_addr)
                // ld_imm %r1, %dtpoff(tls_array_small_LocalDynamic)
                // lda_xi64.l %r1, %r3, %r1, 0, 100
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "(" << object_array << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << dtpoff << "("  << object_array << ")" << std::endl;
                out << CHECK << insn_name(insn_lda_xi64) << ".l %r1, %r3, %r1, 0, " << (100 * test.size) << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel %r3, %tgd(tls_array_small_GlobalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // add_imm_i64 %r1, %r3, 100
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "(" << object_array << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_add_imm_i64) << " %r1, %r3, " << (100 * test.size) << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(tls_array_large_InitialExec_bool) 
                // lda_xi64.l %r1, %tp, %r1, 0, 100 
                out << CHECK << InsnCounter(this, insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "(" << object_array << ")" << std::endl;
                out << CHECK << InsnCounter(this, insn_lda_xi64) << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << std::endl;
                break;
        }

        out << test.memory_type << "* "  << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object_array << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // load TLS variable
        //----------------------------------------------------------------------------
        label = test.create_name("load_tls", cm, tm);
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // ld_u64.l %r2, %tp, %tpoff_ext(x_tls_extern)
                out << CHECK << insn_name(test.base.load) << data_suffix << " %r1, %tp, " << tpoff << "("  << object << ")" << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel.l %r3, %tld_ext(x_tls_extern)
                // call %r2, %jpc(__tls_get_addr)
                // ld_u64.l %r1, %r3, %dtpoff_ext(x_tls_extern)
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "("  << object << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(test.base.load) << data_suffix << " %r1, %r3, " << dtpoff << "("  << object << ")" << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // ld_u64    %r1, %r3, 0
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "("  << object << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(test.base.load) << " %r1, %r3, 0" << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(x_tls_extern)
                // ld_xi64_u64 %r1, %tp, %r1, 0, 0
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "("  << object << ")" << std::endl;
                out << CHECK << insn_name(test.xi64.load) << " %r1, %tp, %r1, 0, 0" << std::endl;
                break;
        }

        out << test.type << " "  << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << object << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // load TLS variable (folded - array[100])
        //----------------------------------------------------------------------------
        out << "// test TLS array folding (load)\n";
        label = test.create_name("load_tls_folded", cm, tm);
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // FIXME(?): ld_u64.l %r2, %tp, %tpoff_ext(x_tls_extern)
                // ld_imm %r1, %tpoff(tls_array_small_LocalExec_i8)
                // ld_xi64_i8.l %r1, %tp, %r1, 0, 100
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << tpoff << "("  << object_array << ")" << std::endl;
                out << CHECK << insn_name(test.xi64.load) << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << std::endl;
                break;
            case LocalDynamic:
                // FIXME(?): lda_iprel.l %r3, %tld_ext(x_tls_extern)
                // FIXME(?): call %r2, %jpc(__tls_get_addr)
                // FIXME(?): ld_u64.l %r1, %r3, %dtpoff_ext(x_tls_extern)

                // lda_iprel.l %r3, %tld_ext(tls_array_large_LocalDynamic_i8) 
                // call.l %r2, %jpc_ext(__tls_get_addr) 
                // ld_imm.l %r1, %dtpoff_ext(tls_array_large_LocalDynamic_i8) 
                // ld_xi64_i8.l %r1, %r3, %r1, 0, 100 

                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "("  << object_array << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << dtpoff << "("  << object_array << ")" << std::endl;
                out << CHECK << InsnCounter(this, test.xi64.load) << ".l %r1, %r3, %r1, 0, " << (100 * test.size) << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // ld_u64 %r1, %r3, 0
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "("  << object_array << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << InsnCounter(this, test.base.load) << " %r1, %r3, " << (100 * test.size) << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(x_tls_extern)
                // ld_xi64_u64 %r1, %tp, %r1, 0, 0
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "("  << object_array << ")" << std::endl;
                out << CHECK << InsnCounter(this, test.xi64.load) << ".l %r1, %tp, %r1, 0, " << (100 * test.size) << std::endl;
                break;
        }

        out << test.type << " " << label << "(" << test.memory_type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << object_array << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // store TLS variable
        //----------------------------------------------------------------------------
        label = test.create_name("store_tls", cm, tm);
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // st_i64.l %r1, %tp, %tpoff_ext(x_tls_extern)
                out << CHECK << insn_name(test.base.store) << data_suffix << " %r1, %tp, " << tpoff << "("  << object << ")" << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel.l %r3, %tld_ext(x_tls_extern)
                // call %r2, %jpc(__tls_get_addr)
                // st_i64.l %r1, %r3, %dtpoff_ext(x_tls_extern)
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "("  << object << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << InsnCounter(this, test.base.store) << data_suffix << " %r1, %r3, " << dtpoff << "("  << object << ")" << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // st_i64 %r1, %r3, 0
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "("  << object << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << InsnCounter(this, test.base.store) << " %r1, %r3, 0" << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r2, %gottpoff_ext(x_tls_extern)
                // st_xi64_i64 %r1, %tp, %r2, 0, 0
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r2, " << gottpoff << "("  << object << ")" << std::endl;
                out << CHECK << InsnCounter(this, test.xi64.store) << " %r1, %tp, %r2, 0, 0" << std::endl;
                break;
        }

        out << "void "  << label << "(" << test.type << " value)" << std::endl;
        out << "{" << std::endl;
        out << "    "  << object << " = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

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

} // namespace llvm
} // namespace postrisc
