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

    if (argc < 3)
    {
        std::cout << "Invalid arguments\n";
        return 0;
    }

    auto aes = CipherFactoryImpl().createCipher(CipherFactory::Algorithm::AES256, 8);

    std::ifstream in(argv[1], std::ios::binary);
    if (!in.good())
    {
        std::cout << "Invalid input file path\n";
    }

    std::ofstream out(argv[2], std::ios::trunc | std::ios::binary);
    if (!out.good())
    {
        std::cout << "Invalid output file path\n";
    }

    auto start = high_resolution_clock::now();
    aes->encrypt(key, in, out);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << '\n';

    in.close();
    out.close();

    std::ifstream in2(argv[2], std::ios::binary);
    std::ofstream out2(std::string("test_") + argv[1]);

    start = high_resolution_clock::now();
    aes->decrypt(key, in2, out2);
    stop = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(stop - start);
    std::cout << duration.count() << '\n';

    in2.close();
    out2.close();

    return 0;
}
