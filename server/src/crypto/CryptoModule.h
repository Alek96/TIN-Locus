#ifndef LOCUS_CRYPTOMODULE_H
#define LOCUS_CRYPTOMODULE_H

#include "Crypto.h"

namespace crypto {
    typedef std::vector<unsigned char> byte_vector;

    class CryptoModule {
    public:
        enum class Algorithm : unsigned int {
            None,
            ServerRSA,
            OuterRSA,
            AES
        };

        CryptoModule() = default;

        void setOuterRSAKey(const byte_vector &key);
        const byte_vector getOuterRSAKey() const;

        byte_vector encryptRSA(RSACrypto *rsaCrypto, const byte_vector &text);
        byte_vector decryptRSA(RSACrypto *rsaCrypto, const byte_vector &cipher);

        byte_vector getSymmetricKey() const;

        void use(const Algorithm &algo);
        byte_vector encrypt(const byte_vector &text);
        byte_vector decrypt(const byte_vector &cipher);

    private:
        RSAServerCrypto rsaServerCrypto;
        RSASimpleCrypto rsaSimpleCrypto;
        AESCrypto aesCrypto;
        
        bool outerRSAKey = false;
        Algorithm algorithm = Algorithm::None;
    };
}


#endif //LOCUS_CRYPTOMODULE_H
