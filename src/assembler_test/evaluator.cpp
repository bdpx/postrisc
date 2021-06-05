#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "assembler/Assembler.hpp"

namespace postrisc
{

class AssemblerEvaluatorTester : public Assembler
{
public:
    AssemblerEvaluatorTester(const Decoder& decoder_)
        : Assembler(decoder_)
    {
        BOOST_TEST_MESSAGE("AssemblerEvaluatorTester setup");
    }

    ~AssemblerEvaluatorTester(void)
    {
        BOOST_TEST_MESSAGE("AssemblerEvaluatorTester teardown");
    }

    void assemble(const std::string& string, unsigned numbits, i64 value);

};

void
AssemblerEvaluatorTester::assemble(const std::string& string, unsigned numbits, i64 value)
{
    Setup();

    std::istringstream input(string);
    TokenizeInput(input);

    init_pass(EAssemblerPass::compile);

    BOOST_TEST(IsNextRowExist());

    i64 const test_value = EvaluateInt(numbits, eval_signed, 0);
    BOOST_TEST(test_value == value);
}

BOOST_AUTO_TEST_CASE(TestEvaluator)
{
   const unsigned numbits = 20;
   u64 const mask = (U64C(1) << numbits) - 1;

   Decoder decoder;
   AssemblerEvaluatorTester tester(decoder);
   tester.assemble("2 + 3", numbits, 5 & mask);
   tester.assemble("2 - 7", numbits, -5 & mask);
   tester.assemble("- 2 * 3", numbits, -6 & mask);
}

} // namespace postrisc
