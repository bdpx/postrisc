#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "util/endianness.hpp"

namespace postrisc {

BOOST_AUTO_TEST_SUITE(test_endian_suite)

BOOST_AUTO_TEST_CASE(test_16)
{
    BOOST_TEST_MESSAGE("test endian change for 16 bits");
    static const struct {
        u16 value;
        u16 expected;
    } test_cases[] = {
        { U16C(0x1234),
          U16C(0x3412) },
    };
    for (auto const& test : test_cases) {
        BOOST_TEST(util::change_endian(test.value) == test.expected);
    }
}

BOOST_AUTO_TEST_CASE(test_32)
{
    BOOST_TEST_MESSAGE("test endian change for 32 bits");
    static const struct {
        u32 value;
        u32 expected;
    } test_cases[] = {
        { U32C(0x12345678),
          U32C(0x78563412) },
    };
    for (auto const& test : test_cases) {
        BOOST_TEST(util::change_endian(test.value) == test.expected);
    }
}

BOOST_AUTO_TEST_CASE(test_64)
{
    BOOST_TEST_MESSAGE("test endian change for 64 bits");
    static const struct {
        u64 value;
        u64 expected;
    } test_cases[] = {
        { U64C(0x1234567890abcdef),
          U64C(0xefcdab9078563412) },
    };
    for (auto const& test : test_cases) {
        BOOST_TEST(util::change_endian(test.value) == test.expected);
    }
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
