#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static const struct {
    unsigned size;
    unsigned scale;
    char type[8];
    char load_type[8];
    char store_type[8];
} types[] = {
    { sizeof(i8),           0, "i8",   "i8",   "i8" },
    { sizeof(i16),          1, "i16",  "i16",  "i16" },
    { sizeof(i32),          2, "i32",  "u32",  "i32" }, //FIXME: load type is u32
    { sizeof(i64),          3, "i64",  "u64",  "i64" }, //FIXME: load type is u64
    { sizeof(i128),         4, "i128", "i128", "i128" },
    { sizeof(u8),           0, "u8",   "u8",   "i8" },
    { sizeof(u16),          1, "u16",  "u16",  "i16" },
    { sizeof(u32),          2, "u32",  "u32",  "i32" },
    { sizeof(u64),          3, "u64",  "u64",  "i64" },
    { sizeof(u128),         4, "u128", "i128", "i128" },
    { sizeof(native::f16),  1, "f16",  "u16",  "i16" },
    { sizeof(native::f32),  2, "f32",  "u32",  "i32" },
    { sizeof(native::f64),  3, "f64",  "u64",  "i64" },
    { sizeof(native::f128), 4, "f128", "i128", "i128" },
};

CODEGEN_HANDLER(Load)
{
    for (const auto& type : types) {
        out << "extern " << type.type << " global_" << type.type << ";" << std::endl;
        out << "// CHECK-LABEL: @load_iprel_" << type.type << std::endl;
        out << type.type << " load_iprel_" << type.type << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: ld_iprel_" << type.load_type << " %r1, %pcfwd(global_" << type.type << ")" << std::endl;
        out << "    return global_" << type.type << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// test folding immediate info iprel relocation\n";
        out << "extern " << type.type << " global_array_" << type.type << "[1000];" << std::endl;
        out << "// CHECK-LABEL: @load_iprel_folded_" << type.type << std::endl;
        out << type.type << " load_iprel_folded_" << type.type << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: ld_iprel_" << type.load_type << " %r1, %pcfwd(global_array_" << type.type << "+" << (type.size * 100) << ")" << std::endl;
        out << "    return global_array_" << type.type << "[100];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// CHECK-LABEL: @load_base_" << type.type << std::endl;
        out << type.type << " load_base_" << type.type << "(" << type.type << " *ptr)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: ld_" << type.load_type << " %r1, %r1, " << type.size * 400 << std::endl;
        out << "    return ptr[400];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// CHECK-LABEL: @load_xi64_" << type.type << std::endl;
        out << type.type << " load_xi64_" << type.type << "(" << type.type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: ld_xi64_" << type.load_type << " %r1, %r1, %r2, " << type.scale << ", " << type.size * 2 << std::endl;
        out << "    return ptr[index+2];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// CHECK-LABEL: @load_xi64_ext_" << type.type << std::endl;
        out << type.type << " load_xi64_ext_" << type.type << "(" << type.type << " *ptr, i64 index)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: ld_xi64_" << type.load_type << ".l %r1, %r1, %r2, " << type.scale << ", " << type.size * 1000 << std::endl;
        out << "    return ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;

        out << "// CHECK-LABEL: @load_xi32_" << type.type << std::endl;
        out << type.type << " load_xi32_" << type.type << "(" << type.type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: ld_xi32_" << type.load_type << " %r1, %r1, %r2, " << type.scale << ", " << type.size * 2 << std::endl;
        out << "    return ptr[index+2];" << std::endl;
        out << "}" << std::endl;

        out << "// CHECK-LABEL: @load_xi32_ext_" << type.type << std::endl;
        out << type.type << " load_xi32_ext_" << type.type << "(" << type.type << " *ptr, i32 index)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: ld_xi32_" << type.load_type << ".l %r1, %r1, %r2, " << type.scale << ", " << type.size * 1000 << std::endl;
        out << "    return ptr[index+1000];" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}


CODEGEN_HANDLER(Store)
{
    for (const auto& type : types) {
        out << "extern " << type.type << " global_" << type.type << ";" << std::endl;

        out << "// CHECK-LABEL: @store_" << type.type << std::endl;
        out << "void store_" << type.type << "(" << type.type << " value, " << type.type << " *ptr, i64 index64, i32 index32)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: st_iprel_" << type.store_type << " %r1, %pcfwd(global_" << type.type << ")" << std::endl;
        out << "    global_" << type.type << " = value;" << std::endl;

        out << "// CHECK: st_" << type.store_type << " %r1, %r2, " << type.size * 400 << std::endl;
        out << "    ptr[400] = value;" << std::endl;

        out << "// CHECK: st_xi64_" << type.store_type << " %r1, %r2, %r3, " << type.scale << ", " << type.size * 1 << std::endl;
        out << "    ptr[index64+1] = value;" << std::endl;

        out << "// CHECK: st_xi32_" << type.store_type << " %r1, %r2, %r4, " << type.scale << ", " << type.size * 1 << std::endl;
        out << "    ptr[index32+1] = value;" << std::endl;

        out << "}" << std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(StoreZero)
{
    for (const auto& type : types) {
        out << "extern " << type.type << " global_" << type.type << ";" << std::endl;

        out << "// CHECK-LABEL: @store_zero_" << type.type << std::endl;
        out << "void store_zero_" << type.type << "(" << type.type << " value, " << type.type << " *ptr, i64 index64, i32 index32, u32 uindex32)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: st_iprel_" << type.store_type << " %gz, %pcfwd(global_" << type.type << ")" << std::endl;
        out << "    global_" << type.type << " = 0;" << std::endl;

        out << "// CHECK: st_" << type.store_type << " %gz, %r2, " << type.size * 400 << std::endl;
        out << "    ptr[400] = 0;" << std::endl;

        out << "// CHECK: st_xi64_" << type.store_type << " %gz, %r2, %r3, " << type.scale << ", " << type.size * 1 << std::endl;
        out << "    ptr[index64+1] = 0;" << std::endl;

        out << "// CHECK: st_xi32_" << type.store_type << " %gz, %r2, %r4, " << type.scale << ", " << type.size * 1 << std::endl;
        out << "    ptr[index32+1] = 0;" << std::endl;

        // FIXME: xu32
        //out << "// CHECK: st_xu32_" << type.store_type << " %gz, %r2, %r4, " << type.scale << ", " << type.size * 1 << std::endl;
        //out << "    ptr[uindex32+1] = 0;" << std::endl;

        out << "}" << std::endl;
        out << std::endl;
    }
}

} // namespace llvm
} // namespace postrisc
