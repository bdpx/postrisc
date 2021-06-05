#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(ClMul)
{
out << R"ABRACADABRA(
// CHECK-LABEL: @test_clmul
v2i64 test_clmul(v2i64 a, v2i64 b)
{
  v2i64 c;
  // CHECK: clmul_ll %r3, %r1, %r2
  c = __builtin_postrisc_clmul_ll(a, b);
  // CHECK: clmul_hl %r1, %r1, %r3
  c = __builtin_postrisc_clmul_hl(a, c);
  // CHECK: clmul_hh %r1, %r1, %r2
  c = __builtin_postrisc_clmul_hh(c, b);
  return c;
}
)ABRACADABRA";
}

} // namespace llvm
} // namespace postrisc
