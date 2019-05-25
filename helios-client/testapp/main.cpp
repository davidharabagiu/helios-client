#include "rsaimpl.h"

#include <iostream>
#include <string>
#include <vector>

int main()
{
    // RsaUtilitiesImpl().generateKeyPair("key.pk", "key.pubk");

    std::string message("Mos Craciun");

    auto encrypted = RsaImpl().encrypt("key.pubk", std::vector<uint8_t>(message.begin(), message.end()));
    auto decrypted = RsaImpl().decrypt("key.pk", encrypted);

    std::string message2(decrypted.begin(), decrypted.end());

    std::cout << message2 << '\n';

    return 0;
}
