#include "cipherfactoryimpl.h"
#include "aes/aescipher.h"

std::shared_ptr<Cipher> CipherFactoryImpl::createCipher(CipherFactory::Algorithm cipherType, int numberOfThreads) const
{
    switch (cipherType)
    {
        case Algorithm::AES128:
        {
            return std::make_shared<Aes::AesCipher>(Aes::KeySize::AES128, numberOfThreads);
        }
        case Algorithm::AES192:
        {
            return std::make_shared<Aes::AesCipher>(Aes::KeySize::AES192, numberOfThreads);
        }
        case Algorithm::AES256:
        {
            return std::make_shared<Aes::AesCipher>(Aes::KeySize::AES256, numberOfThreads);
        }
        default:
        {
            assert(false);
            return nullptr;
        }
    }
}
