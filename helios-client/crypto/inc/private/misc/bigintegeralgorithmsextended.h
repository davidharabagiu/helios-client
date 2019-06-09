#ifndef MISC_BIGINTEGERALGORITHMSEXTENDED_H
#define MISC_BIGINTEGERALGORITHMSEXTENDED_H

#include <BigIntegerLibrary.hh>
#include <memory>

// Forward declarations
class Random;

namespace BigIntegerAlgorithms
{
/**
 * @brief Generate a random in the interval [min, max]
 * @param generator - Generator to use to get random numbers
 * @param min - BigUnsigned
 * @param max - BigUnsigned
 * @param odd - Set this to true to force the generated number to be odd
 * @return BigUnsigned
 */
BigUnsigned random(const std::shared_ptr<Random>& generator, const BigUnsigned& min, const BigUnsigned& max,
                   bool odd = false);

/**
 * @brief Check if a number is prime using the Miller-Rabin test
 * @param generator - Generator to use to get random numbers
 * @param num - BigUnsigned
 * @param numberOfRounds - Number of testing rounds. Determines the accuracy of the test.
 * @return bool
 */
bool isProbablyPrime(const std::shared_ptr<Random>& generator, const BigUnsigned& num, int numberOfRounds);

/**
 * @brief Calculate lowest common multiple
 * @param a - BigUnsigned
 * @param b - BigUnsigned
 * @return BigUnsigned
 */
BigUnsigned lcm(const BigUnsigned& a, const BigUnsigned& b);
}  // namespace BigIntegerAlgorithms

#endif  // MISC_BIGINTEGERALGORITHMSEXTENDED_H
