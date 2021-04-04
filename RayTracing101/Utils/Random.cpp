#include "Utils/Random.h"
#include <cstdint>
#include <random> // for std::random_device only

//-- SquirrelNoise3 from https://www.youtube.com/watch?v=LWFzPP8ZbdU
unsigned int Get1dNoiseUint(int idx, unsigned int seed)
{
    const unsigned int BIT_NOISE1 = 0x68E31DA4; // 0b0110'1000'1110'0011'0001'1101'1010'0100
    const unsigned int BIT_NOISE2 = 0xB5297A4D; // 0b1011'0101'0010'1001'0111'1010'0100'1101
    const unsigned int BIT_NOISE3 = 0x1B56C4E9; // 0b0001'1011'0101'0110'1100'0100'1110'1001

    unsigned int bits = (unsigned int)idx;
    bits *= BIT_NOISE1;
    bits += seed;
    bits ^= (bits >> 8);
    bits += BIT_NOISE2;
    bits ^= (bits << 8);
    bits *= BIT_NOISE3;
    bits ^= (bits >> 8);

    return bits;
}

unsigned int Get2dNoiseUint(int idxX, int idxY, unsigned int seed)
{
    return Get1dNoiseUint(idxX + idxY * 198491317, seed);
}

unsigned int Get3dNoiseUint(int idxX, int idxY, int idxZ, unsigned int seed)
{
    return Get1dNoiseUint(idxX + (idxY * 198491317) + (idxZ * 6542989), seed);
}

//-----------------------------------------------------------------------------------------------

//-- https://experilous.com/1/blog/post/perfect-fast-random-floating-point-numbers#half-open-range
inline
float as_float(uint32_t i)
{
    union
    {
        uint32_t i;
        float f;
    } pun = { i };
    return pun.f;
}

inline
float uint32_to_float(uint32_t x) // [0,1)
{
    return as_float(0x3F800000U | (x >> 9)) - 1.f;
}

float Get1dNoise01(int idx, unsigned int seed)
{
    return uint32_to_float(Get1dNoiseUint(idx, seed));
}

float Get2dNoise01(int idxX, int idxY, unsigned int seed)
{
    return uint32_to_float(Get2dNoiseUint(idxX, idxY, seed));
}

float Get3dNoise01(int idxX, int idxY, int idxZ, unsigned int seed)
{
    return uint32_to_float(Get3dNoiseUint(idxX, idxY, idxZ, seed));
}

//-----------------------------------------------------------------------------------------------
unsigned int m_idx = 0;
unsigned int m_seed = std::random_device() ();

template<>
float GetRandom01<float>()
{
    return Get1dNoise01(m_idx++, m_seed);
}

//-- "Conversion of High-Period Random Numbers to Floating Point", Jurgen Doornik
template<>
double GetRandom01<double>()
{
    unsigned int v = Get1dNoiseUint(m_idx++, m_seed);

    return (int)v * 2.32830643653869628906e-010 + 0.5;
}