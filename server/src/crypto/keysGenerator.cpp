#include "keysGenerator.h"

void crypto::keysGenerator(unsigned int length)  {
    const char* PUBLIC_KEY_FILENAME = "rsa-public.key";
    const char* PRIVATE_KEY_FILENAME = "rsa-private.key";

    CryptoPP::AutoSeededRandomPool randomGenerator;
    CryptoPP::RSA::PrivateKey privateKey;
    CryptoPP::RSA::PublicKey publicKey;

    privateKey.GenerateRandomWithKeySize(randomGenerator, length);
    publicKey.AssignFrom(privateKey);

    if (!publicKey.Validate(randomGenerator, 3)) {
        throw std::runtime_error("RSA public key validation failed");
    }
    if (!privateKey.Validate(randomGenerator, 3)) {
        throw std::runtime_error("RSA private key validation failed");
    }

    // save keys to files
    {
        CryptoPP::ByteQueue queue;
        publicKey.Save(queue);
        CryptoPP::FileSink file(PUBLIC_KEY_FILENAME);
        queue.CopyTo(file);
        file.MessageEnd();
    }
    {
        CryptoPP::ByteQueue queue;
        privateKey.Save(queue);
        CryptoPP::FileSink file(PRIVATE_KEY_FILENAME);
        queue.CopyTo(file);
        file.MessageEnd();
    }
};
