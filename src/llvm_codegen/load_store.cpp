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

static const struct {
    unsigned size;
    unsigned scale;
    char type[8];
    addressing_mode base;
    addressing_mode iprel;
    addressing_mode xi64;
    addressing_mode xu64;
    addressing_mode xi32;
    addressing_mode xu32;
} types[] = {
#define XXX(size, scale, type, load_type, store_type) \
    { size, scale, #type, \
        { insn_ld_       ## load_type, insn_st_       ## store_type }, \
        { insn_ld_iprel_ ## load_type, insn_st_iprel_ ## store_type }, \
        { insn_ld_xi64_  ## load_type, insn_st_xi64_  ## store_type }, \
        { insn_ld_xu64_  ## load_type, insn_st_xu64_  ## store_type }, \
        { insn_ld_xi32_  ## load_type, insn_st_xi32_  ## store_type }, \
        { insn_ld_xu32_  ## load_type, insn_st_xu32_  ## store_type }, \
    },

    XXX(sizeof(bool),         0, bool, u8,   i8)
    XXX(sizeof(i8),           0, i8,   i8,   i8)
    XXX(sizeof(i16),          1, i16,  i16,  i16)
    XXX(sizeof(i32),          2, i32,  u32,  i32)
    XXX(sizeof(i64),          3, i64,  u64,  i64)
    XXX(sizeof(i128),         4, i128, i128, i128)
    XXX(sizeof(u8),           0, u8,   u8,   i8)
    XXX(sizeof(u16),          1, u16,  u16,  i16)
    XXX(sizeof(u32),          2, u32,  u32,  i32)
    XXX(sizeof(u64),          3, u64,  u64,  i64)
    XXX(sizeof(u128),         4, u128, i128, i128)
    XXX(sizeof(native::f16),  1, f16,  u16,  i16)
    XXX(sizeof(native::f32),  2, f32,  u32,  i32)
    XXX(sizeof(native::f64),  3, f64,  u64,  i64)
    XXX(sizeof(native::f128), 4, f128, i128, i128)
#undef XXX
};

CODEGEN_HANDLER(LoadBase)
{
    std::string label;
    for (const auto& type : types) {
        label = std::string("load_base_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
        out << CHECK_NEXT << insn_name(type.base.load) << " %r1, %r1, " << type.size * 400 << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << type.type << " " << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[400];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = std::string("load_xi64_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi64.load) << " %r1, %r1, %r2, " << type.scale << ", " << type.size * 2 << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << type.type << " " << label << "(" << type.type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+2];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = std::string("load_xi64_ext_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi64.load) << ".l %r1, %r1, %r2, " << type.scale << ", " << type.size * 1000 << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << type.type << " " << label << "(" << type.type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = std::string("load_xi32_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi32.load) << " %r1, %r1, %r2, " << type.scale << ", " << type.size * 2 << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << type.type << " " << label << "(" << type.type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+2];" << std::endl;
        out << "}" << std::endl;

        label = std::string("load_xi32_ext_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi32.load) << ".l %r1, %r1, %r2, " << type.scale << ", " << type.size * 1000 << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << type.type << " " << label << "(" << type.type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    return ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(StoreBase)
{
    const int disp = 1;

    for (const auto& type : types) {
        std::string label = std::string("store_base_") + type.type;

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.base.store) << " %r1, %r2, " << (type.size * 400) << std::endl;
        out << CHECK_NEXT << insn_name(type.base.store) << " %gz, %r2, " << type.size * 100 << std::endl;
        out << CHECK_EMPTY << std::endl;
        out << CHECK_NEXT << "! BUNDLE" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << type.type << " value, " << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[400] = value;" << std::endl;
        out << "    ptr[100] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_xi64
        label = std::string("store_xi64_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi64.store) << " %r1, %r2, %r3, " << type.scale << ", " << (type.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << type.type << " value, " << type.type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_zero_xi64
        label = std::string("store_zero_xi64_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi64.store) << " %gz, %r1, %r2, " << type.scale << ", " << (type.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << type.type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_xu64
        label = std::string("store_xu64_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi64.store) << " %r1, %r2, %r3, " << type.scale << ", " << (type.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << type.type << " value, " << type.type << " *ptr, u64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_zero_xu64
        label = std::string("store_zero_xu64_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.xi64.store) << " %gz, %r1, %r2, " << type.scale << ", " << (type.size * disp) << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << type.type << " *ptr, u64 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = 0;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_xu32
        label = std::string("store_xu32_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << insn_name(type.xu32.store) << " %r1, %r2, %r3, " << type.scale << ", "<< disp  << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << type.type << " value, " << type.type << " *ptr, u32 index)" << std::endl;
        out << "{" << std::endl;
        out << "    ptr[index + " << disp << "] = value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        // store_zero_xu32
        label = std::string("store_zero_xu32_") + type.type;
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << insn_name(type.xu32.store) << " %gz, %r1, %r2, " << type.scale << ", " << disp << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << label << "(" << type.type << " *ptr, u32 index)" << std::endl;
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
        std::ostringstream func;
        func << "global_" << CodeModelName(cm) << "_func";
        out << "extern int " << func.str() << "(int arg);" << std::endl;

        std::ostringstream func_ptr;
        func_ptr << "global_" << CodeModelName(cm) << "_func_ptr";
        out << "extern int (*" << func_ptr.str() << ")(int arg);" << std::endl;

        out << "// function pointer materialize: " << insn_name(insn_lda_r) << std::endl;

        std::string label = std::string(insn_name(insn_lda_r)) + std::string("_") + CodeModelName(cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_lda_r) << code_suffix
            << " %r1, " << code_reloc << "(" << func.str() << ")" << std::endl;
        out << "ft " << label << "(void)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << func.str() << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// function call: " << insn_name(insn_call) << std::endl;

        label = std::string(insn_name(insn_call)) + std::string("_") + CodeModelName(cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_call) << code_suffix
            << " %r2, " << code_reloc << "(" << func.str() << ")" << std::endl;
        out << "int " << label << "(int value)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << func.str() << "(value);" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// function ptr call: " << insn_name(insn_call_plt) << std::endl;

        label = std::string(insn_name(insn_call_plt)) + std::string("_") + CodeModelName(cm);

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_call_plt) << data_suffix
            << " %r2, " << data_reloc << "(" << func_ptr.str() << ")" << std::endl;
        out << "int " << label << "(int value)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << func_ptr.str() << "(value);" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = std::string("call_plt_folded_") + CodeModelName(cm);

        out << "// test folding immediate into call_plt\n";

        std::ostringstream func_ptr_table;
        func_ptr_table << "global_" << CodeModelName(cm) << "_func_ptr_table";
        out << "extern ft " << func_ptr_table.str() << "[20];" << std::endl;

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_call_plt) << data_suffix
            << " %r2, " << data_reloc << "(" << func_ptr_table.str() << "+" << (sizeof(void*) * 5) << ")" << std::endl;
        out << "int "  << label << "(int arg)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << func_ptr_table.str() << "[5](arg);" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }

    for (const auto& type : types) {
        std::ostringstream object;
        object << "global_" << CodeModelName(cm) << "_" << type.type;
        out << "extern " << type.type << " " << object.str() << ";" << std::endl;

        std::ostringstream object_array;
        object_array << "global_array_" << CodeModelName(cm) << "_" << type.type;
        out << "extern " << type.type << " " << object_array.str() << "[1000];" << std::endl;

        std::string label = std::string("lda_iprel_") + CodeModelName(cm) + "_" + type.type;

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_lda_iprel) << data_suffix
            << " %r1, " << data_reloc << "(" << object.str() << ")" << std::endl;
        out << type.type << "* " << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object.str() << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = std::string("lda_iprel_folded_") + CodeModelName(cm) + "_" + type.type;

        out << "// test folding immediate into lda iprel\n";
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(insn_lda_iprel) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array.str() << "+" << (type.size * 100) << ")" << std::endl;
        out << type.type << "* "  << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object_array.str() << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = std::string("load_iprel_") + CodeModelName(cm) + "_" + type.type;

        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(type.iprel.load) << data_suffix
            << " %r1, " << data_reloc << "("  << object.str() << ")" << std::endl;
        out << type.type << " "  << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << object.str() << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        label = std::string("load_iprel_folded_") + CodeModelName(cm) + "_" + type.type;

        out << "// test folding immediate info ld_iprel\n";
        out << CHECK_LABEL << label << std::endl;
        out << CHECK << insn_name(type.iprel.load) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array.str() << "+" << (type.size * 100) << ")" << std::endl;
        out << type.type << " " << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << object_array.str() << "[100];" << std::endl;
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

    for (const auto& type : types) {
        std::ostringstream object;
        object << "global_" << CodeModelName(cm) << "_" << type.type;
        out << "extern " << type.type << " " << object.str() << ";" << std::endl;

        std::ostringstream object2;
        object2 << "global2_" << CodeModelName(cm) << "_" << type.type;
        out << "extern " << type.type << " " << object2.str() << ";" << std::endl;

        std::ostringstream object_array;
        object_array << "global_array_" << CodeModelName(cm) << "_" << type.type;
        out << "extern " << type.type << " " << object_array.str() << "[1000];" << std::endl;

        std::string label = std::string("store_iprel_") + CodeModelName(cm) + std::string("_") + std::string(type.type);

        out << CHECK_LABEL << label << std::endl;
        out << "void " << label << "(" << type.type << " value)" << std::endl;
        out << "{" << std::endl;

        out << CHECK << insn_name(type.iprel.store) << data_suffix
            << " %r1, " << data_reloc << "(" << object.str() << ")" << std::endl;
        out << "    " << object.str() << " = value;" << std::endl;

        out << CHECK << insn_name(type.iprel.store) << data_suffix
            << " %r1, " << data_reloc << "(" << object_array.str() << "+" << (type.size * 100) << ")" << std::endl;
        out << "    " << object_array.str() << "[100] = value;" << std::endl;

        out << "// test store-zero" << std::endl;
        out << CHECK << insn_name(type.iprel.store) << data_suffix
            << " %gz, " << data_reloc << "(" << object2.str() << ")" << std::endl;
        out << "    " << object2.str() << " = 0;" << std::endl;

        out << "// test store-zero" << std::endl;
        out << CHECK << insn_name(type.iprel.store) << data_suffix
            << " %gz, " << data_reloc << "(" << object_array.str() << "+" << (type.size * 200) << ")" << std::endl;
        out << "    " << object_array.str() << "[200] = 0;" << std::endl;

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

    for (const auto& type : types) {
        // __thread i64 tls_small_i64;
        std::ostringstream object;
        object << "tls_" << CodeModelName(cm) << "_" << ThreadModelName(tm) << "_" << type.type;
        out << "__thread " << type.type << " " << object.str() << ";" << std::endl;

        // __thread i64 tls_array_small_i64[100];
        std::ostringstream object_array;
        object_array << "tls_array_" << CodeModelName(cm) << "_" << ThreadModelName(tm) << "_" << type.type;
        out << "__thread " << type.type << " " << object_array.str() << "[1000];" << std::endl;

        //----------------------------------------------------------------------------
        // load TLS address
        //----------------------------------------------------------------------------
        std::string label = std::string("lda_tls_") + CodeModelName(cm) + "_" + ThreadModelName(tm) + "_" + type.type;
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // add_imm_i64 %r1, %tp, %tpoff(tls_small_LocalExec_bool)
                out << CHECK << insn_name(insn_add_imm_i64) << data_suffix << " %r1, %tp, " << tpoff << "(" << object.str() << ")" << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel %r3, %tld(tls_small_LocalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // add_imm_i64 %r1, %r3, %dtpoff(tls_small_LocalDynamic_bool)
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "(" << object.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_add_imm_i64) << data_suffix << " %r1, %r3, " << dtpoff << "(" << object.str() << ")" << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel %r3, %tgd(tls_small_GlobalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // mov %r1, %r3
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "(" << object.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_mov) << " %r1, %r3" << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(tls_large_InitialExec_i8)
                // add_i64 %r1, %tp, %r1
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "(" << object.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_add_i64) << " %r1, %tp, %r1" << std::endl;
                break;
        }

        out << type.type << "* " << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object.str() << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // load TLS address (folded)
        //----------------------------------------------------------------------------
        label = std::string("lda_tls_folded_") + CodeModelName(cm) + "_" + ThreadModelName(tm) + "_" + type.type;
        out << "// test TLS array folding (load address)\n";
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // ld_imm.l %r1, %tpoff_ext(tls_array_medium_LocalExec_bool)
                // lda_xi64.l %r1, %tp, %r1, 0, 100
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << tpoff << "("  << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_lda_xi64) << ".l %r1, %tp, %r1, 0, " << (100 * type.size) << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel %r3, %tld(tls_array_small_LocalDynamic)
                // call %r2, %jpc(__tls_get_addr)
                // ld_imm %r1, %dtpoff(tls_array_small_LocalDynamic)
                // lda_xi64.l %r1, %r3, %r1, 0, 100
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "(" << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << dtpoff << "("  << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_lda_xi64) << ".l %r1, %r3, %r1, 0, " << (100 * type.size) << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel %r3, %tgd(tls_array_small_GlobalDynamic_bool)
                // call %r2, %jpc(__tls_get_addr)
                // add_imm_i64 %r1, %r3, 100
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "(" << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_add_imm_i64) << " %r1, %r3, " << (100 * type.size) << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(tls_array_large_InitialExec_bool) 
                // lda_xi64.l %r1, %tp, %r1, 0, 100 
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "(" << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_lda_xi64) << ".l %r1, %tp, %r1, 0, " << (100 * type.size) << std::endl;
                break;
        }

        out << type.type << "* "  << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return &"  << object_array.str() << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // load TLS variable
        //----------------------------------------------------------------------------
        label = std::string("load_tls_") + CodeModelName(cm) + "_" + ThreadModelName(tm) + "_" + type.type;
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // ld_u64.l %r2, %tp, %tpoff_ext(x_tls_extern)
                out << CHECK << insn_name(type.base.load) << data_suffix << " %r1, %tp, " << tpoff << "("  << object.str() << ")" << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel.l %r3, %tld_ext(x_tls_extern)
                // call %r2, %jpc(__tls_get_addr)
                // ld_u64.l %r1, %r3, %dtpoff_ext(x_tls_extern)
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "("  << object.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(type.base.load) << data_suffix << " %r1, %r3, " << dtpoff << "("  << object.str() << ")" << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // ld_u64    %r1, %r3, 0
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "("  << object.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(type.base.load) << " %r1, %r3, 0" << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(x_tls_extern)
                // ld_xi64_u64 %r1, %tp, %r1, 0, 0
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "("  << object.str() << ")" << std::endl;
                out << CHECK << insn_name(type.xi64.load) << " %r1, %tp, %r1, 0, 0" << std::endl;
                break;
        }

        out << type.type << " "  << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return "  << object.str() << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // load TLS variable (folded - array[100])
        //----------------------------------------------------------------------------
        out << "// test TLS array folding (load)\n";
        label = std::string("load_tls_folded_") + CodeModelName(cm) + "_" + ThreadModelName(tm) + "_" + type.type;
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // FIXME(?): ld_u64.l %r2, %tp, %tpoff_ext(x_tls_extern)
                // ld_imm %r1, %tpoff(tls_array_small_LocalExec_i8)
                // ld_xi64_i8.l %r1, %tp, %r1, 0, 100
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << tpoff << "("  << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(type.xi64.load) << ".l %r1, %tp, %r1, 0, " << (100 * type.size) << std::endl;
                break;
            case LocalDynamic:
                // FIXME(?): lda_iprel.l %r3, %tld_ext(x_tls_extern)
                // FIXME(?): call %r2, %jpc(__tls_get_addr)
                // FIXME(?): ld_u64.l %r1, %r3, %dtpoff_ext(x_tls_extern)

                // lda_iprel.l %r3, %tld_ext(tls_array_large_LocalDynamic_i8) 
                // call.l %r2, %jpc_ext(__tls_get_addr) 
                // ld_imm.l %r1, %dtpoff_ext(tls_array_large_LocalDynamic_i8) 
                // ld_xi64_i8.l %r1, %r3, %r1, 0, 100 

                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "("  << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(insn_ld_imm) << data_suffix << " %r1, " << dtpoff << "("  << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(type.xi64.load) << ".l %r1, %r3, %r1, 0, " << (100 * type.size) << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // ld_u64 %r1, %r3, 0
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "("  << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(type.base.load) << " %r1, %r3, " << (100 * type.size) << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r1, %gottpoff_ext(x_tls_extern)
                // ld_xi64_u64 %r1, %tp, %r1, 0, 0
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r1, " << gottpoff << "("  << object_array.str() << ")" << std::endl;
                out << CHECK << insn_name(type.xi64.load) << ".l %r1, %tp, %r1, 0, " << (100 * type.size) << std::endl;
                break;
        }

        out << type.type << " " << label << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << object_array.str() << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        //----------------------------------------------------------------------------
        // store TLS variable
        //----------------------------------------------------------------------------
        label = std::string("store_tls_") + CodeModelName(cm) + "_" + ThreadModelName(tm) + "_" + type.type;
        out << CHECK_LABEL << label << std::endl;

        switch (tm) {
            case LocalExec:
                // st_i64.l %r1, %tp, %tpoff_ext(x_tls_extern)
                out << CHECK << insn_name(type.base.store) << data_suffix << " %r1, %tp, " << tpoff << "("  << object.str() << ")" << std::endl;
                break;
            case LocalDynamic:
                // lda_iprel.l %r3, %tld_ext(x_tls_extern)
                // call %r2, %jpc(__tls_get_addr)
                // st_i64.l %r1, %r3, %dtpoff_ext(x_tls_extern)
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tld << "("  << object.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(type.base.store) << data_suffix << " %r1, %r3, " << dtpoff << "("  << object.str() << ")" << std::endl;
                break;
            case GlobalDynamic:
            case DefaultTLS:
                // lda_iprel.l %r3, %tgd_ext(x_tls_extern)
                // call.l %r2, %jpc_ext(__tls_get_addr)
                // st_i64 %r1, %r3, 0
                out << CHECK << insn_name(insn_lda_iprel) << data_suffix << " %r3, " << tgd << "("  << object.str() << ")" << std::endl;
                out << CHECK << insn_name(insn_call) << code_suffix << " %r2, " << code_reloc << "(__tls_get_addr)" << std::endl;
                out << CHECK << insn_name(type.base.store) << " %r1, %r3, 0" << std::endl;
                break;
            case InitialExec:
                // ld_iprel_u64.l %r2, %gottpoff_ext(x_tls_extern)
                // st_xi64_i64 %r1, %tp, %r2, 0, 0
                out << CHECK << insn_name(insn_ld_iprel_u64) << data_suffix << " %r2, " << gottpoff << "("  << object.str() << ")" << std::endl;
                out << CHECK << insn_name(type.xi64.store) << " %r1, %tp, %r2, 0, 0" << std::endl;
                break;
        }

        out << "void "  << label << "(" << type.type << " value)" << std::endl;
        out << "{" << std::endl;
        out << "    "  << object.str() << " = value;" << std::endl;
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
