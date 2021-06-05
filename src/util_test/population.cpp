#include <boost/test/unit_test.hpp>

#include "util/common.hpp"

namespace postrisc {

BOOST_AUTO_TEST_SUITE(test_population_suite)

template<typename T>
void common_test()
{
    BOOST_TEST_MESSAGE("test population for " << sizeof(T) << " bits");

    unsigned const numbits = 8 * sizeof(T);

    for (unsigned bitnum = 0; bitnum < numbits; ++bitnum) {
        const T flag = T(1) << bitnum;
        BOOST_TEST(util::population::func(flag) == 1, "one bit flag, bitnum=" << bitnum);
        BOOST_TEST(util::population::func(~flag) == numbits-1, "one zero bitmask, bitnum=" << bitnum);

        const T mask = flag - 1;
        BOOST_TEST(util::population::func(mask) == bitnum, "mask with ones, bitnum=" << bitnum);
        BOOST_TEST(util::population::func(~mask) == numbits-bitnum, "inverted mask, bitnum=" << bitnum);
    }

    BOOST_TEST(util::population::func(T(0)) == 0);
    BOOST_TEST(util::population::func(~T(0)) == numbits);
}

BOOST_AUTO_TEST_CASE(test_32)
{
    common_test<u32>();

    BOOST_TEST(util::population::func(U32C(0x33333333)) == 16);
    BOOST_TEST(util::population::func(U32C(0x55555555)) == 16);
}

BOOST_AUTO_TEST_CASE(test_64)
{
    common_test<u64>();

    BOOST_TEST(util::population::func(U64C(0x3333333333333333)) == 32);
    BOOST_TEST(util::population::func(U64C(0x5555555555555555)) == 32);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
