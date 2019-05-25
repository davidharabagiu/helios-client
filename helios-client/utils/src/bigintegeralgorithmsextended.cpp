#include <random>

#include "bigintegeralgorithmsextended.h"
#include "typeconversions.h"

namespace BigIntegerAlgorithms
{
BigUnsigned random(const BigUnsigned& min, const BigUnsigned& max, bool odd)
{
    static std::random_device                     randomDevice;
    static std::mt19937                           mersenneTwister(randomDevice());
    static std::uniform_int_distribution<uint8_t> distribution;

    const auto& lim        = max - min;
    auto        totalBytes = lim.getLength() * sizeof(BigUnsigned::Blk);
    auto        totalBits  = totalBytes * 8;

    BigUnsigned zeroBitMask;
    for (unsigned i = 0; i < totalBytes; ++i)
    {
        zeroBitMask.setBlock(i, static_cast<BigUnsigned::Blk>(-1));
    }

    for (auto i = safe_integral_cast<unsigned>(totalBits - 1);; --i)
    {
        if (lim.getBit(i))
        {
            break;
        }
        zeroBitMask.setBit(i, false);
        if (i == 0)
        {
            break;
        }
    }

    for (;;)
    {
        BigUnsigned randomNumber;
        for (unsigned long i = 0; i < totalBytes; ++i)
        {
            randomNumber <<= 8;
            randomNumber |= distribution(mersenneTwister);
        }
        randomNumber &= zeroBitMask;
        if (randomNumber <= lim)
        {
            auto ret = randomNumber + min;
            if (odd)
            {
                ret.setBit(0, true);
            }
            return ret;
        }
    }
}

bool isProbablyPrime(const BigUnsigned& num, int numberOfRounds)
{
    auto num_m1 = num - 1;
    auto num_m2 = num - 2;
    auto d      = num_m1;
    int  r      = 1;
    while (!d.getBit(0))
    {
        d >>= 1;
        ++r;
    }
    for (int i = 0; i != numberOfRounds; ++i)
    {
        const auto& a = random(2, num_m2);
        auto        x = modexp(a, d, num);
        if (x == 1 || x == num_m1)
        {
            continue;
        }
        for (int j = 1; j != r; ++j)
        {
            x = modexp(x, 2, num);
            if (x == num_m1)
            {
                continue;
            }
        }
        return false;
    }
    return true;
}

BigUnsigned lcm(const BigUnsigned& a, const BigUnsigned& b)
{
    const auto& g = gcd(a, b);
    return g.isZero() ? g : (a / g * b);
}

}  // namespace BigIntegerAlgorithms
