#include <BigIntegerLibrary.hh>
#include <QByteArray>
#include <QFile>
#include <algorithm>

#include "rsaimpl.h"
#include "typeconversions.h"
#include "bigintegeralgorithmsextended.h"
#include "random.h"
#include "randomfactoryimpl.h"

const BigUnsigned RsaImpl::s_kPublicExponent      = BigUnsigned(65537);
const int         RsaImpl::s_kKeyLength           = 1024;
const int         RsaImpl::s_kPrimalityTestRounds = 6;

RsaImpl::RsaImpl()
    : m_rng(RandomFactoryImpl().isaac64())
{
}

void RsaImpl::generateKeyPair(const std::string& privateKeyFile, const std::string& publicKeyFile) const
{
    BigUnsigned p, q, lambda;
    do
    {
        p      = randomPrime();
        q      = randomPrime();
        lambda = BigIntegerAlgorithms::lcm(p - 1, q - 1);
    } while (gcd(s_kPublicExponent, lambda) != 1 ||
             ((BigInteger(p) - q).getMagnitude() >> (s_kKeyLength / 2 - 100)).isZero());

    BigUnsigned n = p * q;
    BigUnsigned d = modinv(s_kPublicExponent, lambda);

    auto pN           = reinterpret_cast<NumberlikeArray<unsigned long>*>(&n);
    auto nData        = QByteArray::fromRawData(reinterpret_cast<const char*>(pN->blk),
                                         safe_integral_cast<int>(pN->len * sizeof(BigUnsigned::Blk)));
    auto _nDataLength = nData.length();
    auto nDataLength  = QByteArray::fromRawData(reinterpret_cast<const char*>(&_nDataLength), sizeof(_nDataLength));

    auto pE           = reinterpret_cast<const NumberlikeArray<unsigned long>*>(&s_kPublicExponent);
    auto eData        = QByteArray::fromRawData(reinterpret_cast<const char*>(pE->blk),
                                         safe_integral_cast<int>(pE->len * sizeof(BigUnsigned::Blk)));
    auto _eDataLength = eData.length();
    auto eDataLength  = QByteArray::fromRawData(reinterpret_cast<const char*>(&_eDataLength), sizeof(_eDataLength));

    auto pD           = reinterpret_cast<NumberlikeArray<unsigned long>*>(&d);
    auto dData        = QByteArray::fromRawData(reinterpret_cast<const char*>(pD->blk),
                                         safe_integral_cast<int>(pD->len * sizeof(BigUnsigned::Blk)));
    auto _dDataLength = dData.length();
    auto dDataLength  = QByteArray::fromRawData(reinterpret_cast<const char*>(&_dDataLength), sizeof(_dDataLength));

    // Construct private key
    QByteArray privateKey;
    privateKey.append(dDataLength);
    privateKey.append(dData);
    privateKey.append(nDataLength);
    privateKey.append(nData);
    QFile _privateKeyFile(QString::fromStdString(privateKeyFile));
    _privateKeyFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    _privateKeyFile.write(privateKey.toBase64());
    _privateKeyFile.close();

    // Construct public key
    QByteArray publicKey;
    publicKey.append(nDataLength);
    publicKey.append(nData);
    publicKey.append(eDataLength);
    publicKey.append(eData);
    QFile _publicKeyFile(QString::fromStdString(publicKeyFile));
    _publicKeyFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    _publicKeyFile.write(publicKey.toBase64());
    _publicKeyFile.close();
}

std::vector<uint8_t> RsaImpl::encrypt(const std::string& publicKeyFile, const std::vector<uint8_t>& message) const
{
    QFile _publicKeyFile(QString::fromStdString(publicKeyFile));
    _publicKeyFile.open(QIODevice::ReadOnly);
    auto publicKey = QByteArray::fromBase64(_publicKeyFile.readAll());
    _publicKeyFile.close();

    int         nDataLength = *reinterpret_cast<const int*>(publicKey.mid(0, sizeof(int)).data());
    BigUnsigned n(reinterpret_cast<const BigUnsigned::Blk*>(publicKey.mid(sizeof(int), nDataLength).data()),
                  safe_integral_cast<BigUnsigned::Index>(safe_integral_cast<unsigned long>(nDataLength) /
                                                         sizeof(BigUnsigned::Blk)));

    int eDataLength = *reinterpret_cast<const int*>(
        publicKey.mid(safe_integral_cast<int>(sizeof(int)) + nDataLength, sizeof(int)).data());
    BigUnsigned e(reinterpret_cast<const BigUnsigned::Blk*>(
                      publicKey.mid(safe_integral_cast<int>(2 * sizeof(int)) + nDataLength, eDataLength).data()),
                  safe_integral_cast<BigUnsigned::Index>(safe_integral_cast<unsigned long>(eDataLength) /
                                                         sizeof(BigUnsigned::Blk)));

    size_t blockSize = s_kKeyLength / 8;

    std::vector<uint8_t> result;
    result.resize((message.size() % blockSize == 0) ? message.size() : (message.size() / blockSize + 1) * blockSize);

    for (size_t i = 0; i < message.size(); i += blockSize)
    {
        BigInteger block;
        if (i + blockSize > message.size())
        {
            uint8_t* blockData = new uint8_t[blockSize];
            std::fill_n(blockData, blockSize, 0);
            std::copy_n(message.data() + i, message.size() - i, blockData);
            block = BigInteger(reinterpret_cast<const BigUnsigned::Blk*>(blockData),
                               safe_integral_cast<BigInteger::Index>(blockSize / sizeof(BigUnsigned::Blk)));
            delete[] blockData;
        }
        else
        {
            block = BigInteger(reinterpret_cast<const BigUnsigned::Blk*>(message.data() + i),
                               safe_integral_cast<BigInteger::Index>(blockSize / sizeof(BigUnsigned::Blk)));
        }

        auto encryptedBlock  = modexp(block, e, n);
        auto pEncryptedBlock = reinterpret_cast<NumberlikeArray<unsigned long>*>(&encryptedBlock);
        std::copy_n(reinterpret_cast<const uint8_t*>(pEncryptedBlock->blk), blockSize, result.data() + i);
    }

    return result;
}

std::vector<uint8_t> RsaImpl::decrypt(const std::string& privateKeyFile, const std::vector<uint8_t>& message) const
{
    QFile _privateKeyFile(QString::fromStdString(privateKeyFile));
    _privateKeyFile.open(QIODevice::ReadOnly);
    auto privateKey = QByteArray::fromBase64(_privateKeyFile.readAll());
    _privateKeyFile.close();

    int         dDataLength = *reinterpret_cast<const int*>(privateKey.mid(0, sizeof(int)).data());
    BigUnsigned d(reinterpret_cast<const BigUnsigned::Blk*>(privateKey.mid(sizeof(int), dDataLength).data()),
                  safe_integral_cast<BigUnsigned::Index>(safe_integral_cast<unsigned long>(dDataLength) /
                                                         sizeof(BigUnsigned::Blk)));

    int nDataLength = *reinterpret_cast<const int*>(
        privateKey.mid(safe_integral_cast<int>(sizeof(int)) + dDataLength, sizeof(int)).data());
    BigUnsigned n(reinterpret_cast<const BigUnsigned::Blk*>(
                      privateKey.mid(safe_integral_cast<int>(2 * sizeof(int)) + dDataLength, nDataLength).data()),
                  safe_integral_cast<BigUnsigned::Index>(safe_integral_cast<unsigned long>(nDataLength) /
                                                         sizeof(BigUnsigned::Blk)));

    size_t blockSize = s_kKeyLength / 8;

    std::vector<uint8_t> result;
    result.resize(blockSize);

    for (size_t i = 0; i < message.size(); i += blockSize)
    {
        BigInteger block(reinterpret_cast<const BigUnsigned::Blk*>(message.data() + i),
                         safe_integral_cast<BigInteger::Index>(blockSize / sizeof(BigUnsigned::Blk)));

        auto decryptedBlock  = modexp(block, d, n);
        auto pDecryptedBlock = reinterpret_cast<NumberlikeArray<unsigned long>*>(&decryptedBlock);
        std::copy_n(reinterpret_cast<const uint8_t*>(pDecryptedBlock->blk), blockSize, result.data() + i);
    }

    return result;
}

BigUnsigned RsaImpl::randomPrime() const
{
    const auto& min = BigUnsigned(6074001) << (s_kKeyLength / 2 - 30);
    const auto& max = (BigUnsigned(1) << (s_kKeyLength / 2)) - 1;
    for (;;)
    {
        const auto& num = BigIntegerAlgorithms::random(m_rng, min, max, true);
        if (BigIntegerAlgorithms::isProbablyPrime(m_rng, num, s_kPrimalityTestRounds))
        {
            return num;
        }
    }
}
