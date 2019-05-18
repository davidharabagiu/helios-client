#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

#include "cipher.h"
#include "cipherfactoryimpl.h"

using namespace std::chrono;

int main(int argc, char** argv)
{
    static const uint8_t key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                                    0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                    0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    static const uint64_t bufferSize = 16 * 1024 * 1024;

    if (argc < 3)
    {
        std::cout << "Invalid arguments\n";
        return 0;
    }

    uint8_t* buffer  = new uint8_t[bufferSize];
    uint8_t* buffer2 = new uint8_t[bufferSize];

    auto aes = CipherFactoryImpl().createCipher(CipherFactory::Algorithm::AES256, 8);
    aes->setKey(key);

    std::ifstream fin(argv[1], std::ios::binary);
    std::ofstream fout(argv[2], std::ios::binary | std::ios::trunc);

    auto start = high_resolution_clock::now();

    while (!fin.eof())
    {
        fin.read(reinterpret_cast<char*>(buffer), static_cast<std::__1::streamsize>(bufferSize));
        uint64_t lastRead = static_cast<uint64_t>(fin.gcount());
        aes->encrypt(buffer, lastRead, buffer2);
        if (lastRead % 16 != 0)
        {
            lastRead = (lastRead / 16 + 1) * 16;
        }
        fout.write(reinterpret_cast<char*>(buffer2), static_cast<std::__1::streamsize>(lastRead));
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << '\n';

    fin.close();
    fout.close();

    std::ifstream fin2(argv[2], std::ios::binary);
    std::ofstream fout2(std::string("test_") + argv[1], std::ios::binary | std::ios::trunc);

    start = high_resolution_clock::now();

    while (!fin2.eof())
    {
        fin2.read(reinterpret_cast<char*>(buffer), static_cast<std::__1::streamsize>(bufferSize));
        uint64_t lastRead = static_cast<uint64_t>(fin2.gcount());
        aes->decrypt(buffer, lastRead, buffer2);
        fout2.write(reinterpret_cast<char*>(buffer2), static_cast<std::__1::streamsize>(lastRead));
    }

    stop = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << '\n';

    fin2.close();
    fout2.close();

    delete[] buffer;
    delete[] buffer2;

    return 0;
}
