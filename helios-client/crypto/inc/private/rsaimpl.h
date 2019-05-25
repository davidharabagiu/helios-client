#ifndef RSAIMPL_H
#define RSAIMPL_H

#include <BigIntegerLibrary.hh>

#include "rsa.h"

// Forward declarations
class Random;

/**
 * @class RsaImpl
 * @brief RsaUtilities home-made implementation
 */
class RsaImpl : public Rsa
{
public:
    /**
     * @brief Constructor
     */
    RsaImpl();

public:  // from Rsa
    void generateKeyPair(const std::string& privateKeyFile, const std::string& publicKeyFile) const override;
    std::vector<uint8_t> encrypt(const std::string& publicKeyFile, const std::vector<uint8_t>& message) const override;
    std::vector<uint8_t> decrypt(const std::string& privateKeyFile, const std::vector<uint8_t>& message) const override;

private:
    /**
     * @brief Generate a random k-bit prime number greater than sqrt(2) * 2^(k-1)
     * @return BigUnsigned
     */
    BigUnsigned randomPrime() const;

private:
    /**
     * @brief Random number generator instance
     */
    std::shared_ptr<Random> m_rng;

    /**
     * @brief Fixed public exponent
     */
    static const BigUnsigned s_kPublicExponent;

    /**
     * @brief Key length (modulus) in bits
     */
    static const int s_kKeyLength;

    /**
     * @brief Number of rounds for the Miller-Rabin primality test
     */
    static const int s_kPrimalityTestRounds;
};

#endif  // RSAIMPL_H
