#include "util/common.hpp"
#include "util/endianness.hpp"

namespace postrisc {

BOOST_AUTO_TEST_SUITE(test_endian_suite)

BOOST_AUTO_TEST_CASE(test_16)
{
    BOOST_TEST_MESSAGE("test endian change for 16 bits");
    static const struct {
        uint16_t value;
        uint16_t expected;
    } test_cases[] = {
        { UINT16_C(0x1234),
          UINT16_C(0x3412) },
    };
    for (auto const& test : test_cases) {
        BOOST_TEST(util::change_endian(test.value) == test.expected);
    }
}

BOOST_AUTO_TEST_CASE(test_32)
{
    BOOST_TEST_MESSAGE("test endian change for 32 bits");
    static const struct {
        uint32_t value;
        uint32_t expected;
    } test_cases[] = {
        { UINT32_C(0x12345678),
          UINT32_C(0x78563412) },
    };
    for (auto const& test : test_cases) {
        BOOST_TEST(util::change_endian(test.value) == test.expected);
    }
}

BOOST_AUTO_TEST_CASE(test_64)
{
    BOOST_TEST_MESSAGE("test endian change for 64 bits");
    static const struct {
        uint64_t value;
        uint64_t expected;
    } test_cases[] = {
        { UINT64_C(0x1234567890abcdef),
          UINT64_C(0xefcdab9078563412) },
    };
    for (auto const& test : test_cases) {
        BOOST_TEST(util::change_endian(test.value) == test.expected);
    }
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace postrisc
