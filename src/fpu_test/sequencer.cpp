#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

/********************************************************************
* Test sequencer
********************************************************************/

void
CSequence::init(uint64_t seed)
{
    term2Num = 0;
    term1Num = 0;
    expNum = 0;
    done = false;
    generator.seed(seed);
}

uint32_t
CSequence::u32_p1(uint8_t nbits, uint16_t index)
{
    int bitnum = (index >> 2) % (nbits + 1);
    uint32_t value =  1 << bitnum;

    if (index & 1) value = -value;
    if (index & 2) value = ~value;

    uint32_t mask = (1 << nbits) - 1;
    value = value & mask;
    return value;
}

void
CSequence::NextP1(int32_t & z)
{
    z = u32_p1(32, term1Num);
    ++term1Num;
    if (i32NumP1 <= term1Num)
    {
        term1Num = 0;
        done = true;
    }
}

void
CSequence::NextP2(int32_t & z)
{
    z = u32_p1(32, term1Num) + u32_p1(32, term2Num);
    ++term2Num;
    if (i32NumP1 <= term2Num)
    {
        ++term1Num;
        if (i32NumP1 <= term1Num)
        {
            term1Num = 0;
            done = true;
        }
        term2Num = term1Num;
    }
}

void
CSequence::RandomP3(int32_t & z)
{
    z = u32_p1(32) + u32_p1(32) + u32_p1(32);
}

void
CSequence::RandomPInf(int32_t & z)
{
    int n = random8() % 29;
    uint32_t mask = -uint32_t(1) >> n;
    uint32_t offset = -uint32_t(1) << (31 - n);
    z = ( random32() & mask ) + offset;
}

void
CSequence::NextP1(int64_t & z)
{
    z = u64_p1(64, term1Num);
    ++term1Num;
    if (i64NumP1 <= term1Num)
    {
        term1Num = 0;
        done = true;
    }
}

void
CSequence::NextP2(int64_t & z)
{
    z = u64_p1(64, term1Num) + u64_p1(64, term2Num);
    ++term2Num;
    if (i64NumP1 <= term2Num)
    {
        ++term1Num;
        if (i64NumP1 <= term1Num)
        {
            term1Num = 0;
            done = true;
        }
        term2Num = term1Num;
    }
}

void
CSequence::RandomP3(int64_t & z)
{
    z = u64_p1(64) + u64_p1(64) + u64_p1(64);
}

void
CSequence::RandomPInf(int64_t & z)
{
    int n = random8() % 61;
    uint64_t mask = -uint64_t(1) >> n;
    uint64_t offset = -uint64_t(1) << (63 - n);
    z = ( random64() & mask ) + offset;
}

size_t
CSequence::GetTotal(int level, int32_t & )
{
    return level == 1 ? 3 * i32NumP1 : 2 * i32NumP2;
}

size_t
CSequence::GetTotal(int level, int64_t & )
{
     return level == 1 ? 3 * i64NumP1 : 2 * i64NumP2;
}

} // namespace fpu
} // namespace postrisc
