#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/files.h>

#include <stdexcept>
#include <iostream>

const char* PUBLIC_KEY_FILENAME = "rsa1024-public.key";
const char* PRIVATE_KEY_FILENAME = "rsa1024-private.key";

using namespace CryptoPP;

int main()
{
    AutoSeededRandomPool randomGenerator;
    RSA::PrivateKey privateKey;
    RSA::PublicKey publicKey;

    privateKey.GenerateRandomWithKeySize(randomGenerator, 1024);
    publicKey.AssignFrom(privateKey);

    if (!publicKey.Validate(randomGenerator, 3)) {
        std::cerr << "RSA public key validation failed\n";
        return 1;
    }
    if (!privateKey.Validate(randomGenerator, 3)) {
        std::cerr << "RSA private key validation failed\n";
        return 2;
    }

    // save keys to files
    {
        ByteQueue queue;
        publicKey.Save(queue);
        FileSink file(PUBLIC_KEY_FILENAME);
        queue.CopyTo(file);
        file.MessageEnd();
    }
    {
        ByteQueue queue;
        privateKey.Save(queue);
        FileSink file(PRIVATE_KEY_FILENAME);
        queue.CopyTo(file);
        file.MessageEnd();
    }

    std::cout << "RSA1024 Keys generated\n";

    return 0;
}