#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

/********************************************************************
* Test sequencer
********************************************************************/

void
TestSequencer::init(u64 seed)
{
    term2Num = 0;
    term1Num = 0;
    expNum = 0;
    done = false;
    generator.seed(seed);
}

u32
TestSequencer::u32_p1(u8 nbits, u16 index)
{
    int bitnum = (index >> 2) % (nbits + 1);
    u32 value =  1 << bitnum;

    if (index & 1) value = -value;
    if (index & 2) value = ~value;

    u32 mask = (1 << nbits) - 1;
    value = value & mask;
    return value;
}

void
TestSequencer::NextP1(i32 & z)
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
TestSequencer::NextP2(i32 & z)
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
TestSequencer::RandomP3(i32 & z)
{
    z = u32_p1(32) + u32_p1(32) + u32_p1(32);
}

void
TestSequencer::RandomPInf(i32 & z)
{
    int n = random8() % 29;
    u32 mask = -u32(1) >> n;
    u32 offset = -u32(1) << (31 - n);
    z = ( random32() & mask ) + offset;
}

void
TestSequencer::NextP1(i64 & z)
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
TestSequencer::NextP2(i64 & z)
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

void TestSequencer::RandomP3(i64 & z)
{
    z = u64_p1(64) + u64_p1(64) + u64_p1(64);
}

void TestSequencer::RandomPInf(i64 & z)
{
    int n = random8() % 61;
    u64 mask = -u64(1) >> n;
    u64 offset = -u64(1) << (63 - n);
    z = ( random64() & mask ) + offset;
}

size_t TestSequencer::GetTotal(int level, i32 & )
{
    return level == 1 ? 3 * i32NumP1 : 2 * i32NumP2;
}

size_t TestSequencer::GetTotal(int level, i64 & )
{
     return level == 1 ? 3 * i64NumP1 : 2 * i64NumP2;
}

} // namespace fpu
} // namespace postrisc
