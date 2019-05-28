#include "rsaimpl.h"

#include <iostream>
#include <string>
#include <vector>
#include <QByteArray>
#include <QDebug>

int main()
{
    // RsaUtilitiesImpl().generateKeyPair("key.pk", "key.pubk");

    auto encrypted = RsaImpl().encrypt(
        "fgh.pubk", std::vector<uint8_t>{0xab, 0x9e, 0x07, 0x82, 0x23, 0xcc, 0x1f, 0x50, 0x02, 0x5a, 0xa9,
                                         0xfa, 0x49, 0x6b, 0xd7, 0xd5, 0x8b, 0xdb, 0x36, 0x85, 0x46, 0xda,
                                         0xa0, 0x90, 0xee, 0xec, 0x9b, 0x86, 0x28, 0x2f, 0x41, 0xdf});

    auto encryptedBase64 =
        QByteArray::fromRawData(reinterpret_cast<const char*>(encrypted.data()), static_cast<int>(encrypted.size()))
            .toBase64();

    std::cout << encryptedBase64.toStdString() << std::endl;

    auto decrypted = RsaImpl().decrypt("fgh.pk", encrypted);

    std::string message2(decrypted.begin(), decrypted.end());

    std::cout << message2 << '\n';

    return 0;
}
