#include "NumberUtil.h"
#include <cstdint>

NumberUtil::NumberUtil() { }

int NumberUtil::bitCount(int value)
{
    uint32_t M1 = 0x55555555;
    uint32_t M2 = 0x33333333;
    uint32_t M4 = 0x0F0F0F0F;
    uint32_t M8 = 0x00FF00FF;
    value = ((value >> 1) & M1) + (value & M1);
    value = ((value >> 2) & M2) + (value & M2);
    value = ((value >> 4) & M4) + (value & M4);
    value = ((value >> 8) & M8) + (value & M8);
    return (value >> 16) + (value & 0x0000FFFF);
}
