#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "assembler/assembler.hpp"

namespace postrisc
{

class CAssemblerEvaluatorTester : public CAssembler
{
public:
    CAssemblerEvaluatorTester(const Decoder& decoder_)
        : CAssembler(decoder_)
    {
        BOOST_TEST_MESSAGE( "CAssemblerEvaluatorTester setup" );
    }

    ~CAssemblerEvaluatorTester(void)
    {
        BOOST_TEST_MESSAGE( "CAssemblerEvaluatorTester teardown" );
    }

    void assemble(const std::string & string, unsigned numbits, int64_t value);

};

void
CAssemblerEvaluatorTester::assemble(const std::string & string, unsigned numbits, int64_t value)
{
    Setup();

    std::istringstream input(string);
    TokenizeInput(input);

    init_pass(EAssemblerPass::compile);

    BOOST_TEST(IsNextRowExist());

    int64_t const test_value = EvaluateInt(numbits, eval_signed, 0);
    BOOST_TEST(test_value == value);
}

BOOST_AUTO_TEST_CASE(TestEvaluator)
{
   const unsigned numbits = 20;
   uint64_t const mask = (UINT64_C(1) << numbits) - 1;

   Decoder decoder;
   CAssemblerEvaluatorTester tester(decoder);
   tester.assemble("2 + 3", numbits, 5 & mask);
   tester.assemble("2 - 7", numbits, -5 & mask);
   tester.assemble("- 2 * 3", numbits, -6 & mask);
}

} // namespace postrisc
