// Generate an RSA key pair, sign a message and verify it using crypto++ 5.6.1 or later.
// By Tim Sheerman-Chase, 2013
// This code is in the public domain and CC0
// To compile: g++ gen.cpp -lcrypto++ -o gen

#include <string>
using namespace std;
#include <rsa.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>
#include <cstdint>
using namespace CryptoPP;

void GenKeyPair()
{
    // InvertibleRSAFunction is used directly only because the private key
    // won't actually be used to perform any cryptographic operation;
    // otherwise, an appropriate typedef'ed type from rsa.h would have been used.
    AutoSeededRandomPool  rng;
    InvertibleRSAFunction privkey;
    privkey.Initialize(rng, 1024);

    // With the current version of Crypto++, MessageEnd() needs to be called
    // explicitly because Base64Encoder doesn't flush its buffer on destruction.
    Base64Encoder privkeysink(new FileSink("privkey.txt"));
    privkey.DEREncode(privkeysink);
    privkeysink.MessageEnd();

    // Suppose we want to store the public key separately,
    // possibly because we will be sending the public key to a third party.
    RSAFunction pubkey(privkey);

    Base64Encoder pubkeysink(new FileSink("pubkey.txt"));
    pubkey.DEREncode(pubkeysink);
    pubkeysink.MessageEnd();
}

void Sign()
{
    string strContents = "A message to be signed";
    // FileSource("tobesigned.dat", true, new StringSink(strContents));

    AutoSeededRandomPool rng;

    // Read private key
    CryptoPP::ByteQueue bytes;
    FileSource          file("privkey.txt", true, new Base64Decoder);
    file.TransferTo(bytes);
    bytes.MessageEnd();
    RSA::PrivateKey privateKey;
    privateKey.Load(bytes);

    // Sign message
    RSASSA_PKCS1v15_SHA_Signer privkey(privateKey);
    SecByteBlock               sbbSignature(privkey.SignatureLength());
    privkey.SignMessage(rng, (uint8_t const*)strContents.data(), strContents.size(), sbbSignature);

    // Save result
    FileSink sink("signed.dat");
    sink.Put((uint8_t const*)strContents.data(), strContents.size());
    FileSink sinksig("sig.dat");
    sinksig.Put(sbbSignature, sbbSignature.size());
}

void Verify()
{
    // Read public key
    CryptoPP::ByteQueue bytes;
    FileSource          file("pubkey.txt", true, new Base64Decoder);
    file.TransferTo(bytes);
    bytes.MessageEnd();
    RSA::PublicKey pubKey;
    pubKey.Load(bytes);

    RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);

    // Read signed message
    string signedTxt;
    FileSource("signed.dat", true, new StringSink(signedTxt));
    string sig;
    FileSource("sig.dat", true, new StringSink(sig));

    string combined(signedTxt);
    combined.append(sig);

    // Verify signature
    try
    {
        StringSource(combined, true,
                     new SignatureVerificationFilter(verifier, NULL, SignatureVerificationFilter::THROW_EXCEPTION));
        cout << "Signature OK" << endl;
    }
    catch (SignatureVerificationFilter::SignatureVerificationFailed& err)
    {
        cout << err.what() << endl;
    }
}

int main()
{
    GenKeyPair();
    Sign();
    Verify();
}
