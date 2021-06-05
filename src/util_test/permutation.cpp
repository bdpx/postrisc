#include "util/common.hpp"

namespace postrisc {

BOOST_AUTO_TEST_SUITE(test_permutation_suite)

BOOST_AUTO_TEST_CASE(test_32)
{
    BOOST_TEST_MESSAGE("test permutation for 32 bits");
    BOOST_TEST(util::permute::func(UINT32_C(0x12345678), 4|8|16) == UINT32_C(0x87654321));
}

BOOST_AUTO_TEST_CASE(test_64)
{
    BOOST_TEST_MESSAGE("test permutation for 64 bits");
    BOOST_TEST(util::permute::func(UINT64_C(0x1234567890ABCDEF), 4|8|16|32) == UINT64_C(0xFEDCBA0987654321));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
