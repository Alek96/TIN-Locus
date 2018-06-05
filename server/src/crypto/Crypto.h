#ifndef CRYPTO_H
#define CRYPTO_H

#define CRYPTO_ALGORITHM_RSAES_PKCS1
//#define CRYPTO_ALGORITHM_RSAES_OAEP_SHA1
//#define CRYPTO_ALGORITHM_RSAES_OAEP_SHA256

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

#include <vector>
#include <string>

using namespace CryptoPP;

namespace crypto {
    typedef std::vector<unsigned char> byte_vector;

    class RSACrypto {
    public:
        virtual byte_vector encrypt(const byte_vector &text) = 0;
        virtual byte_vector decrypt(const byte_vector &cipher) = 0;

        virtual size_t getMaxPlainTextLength_() const = 0;
        virtual size_t getFixedCipherTextLength_() const = 0;
    };
    
    class RSASimpleCrypto : public RSACrypto {
    public:
        RSASimpleCrypto();
        RSASimpleCrypto(const byte_vector &encryptKey, const byte_vector &decryptKey);

        byte_vector encrypt(const byte_vector &text) override;
        byte_vector decrypt(const byte_vector &cipher) override;

        void setEncryptionKey(const byte_vector &encryptKey);
        void setDecryptionKey(const byte_vector &decryptKey);
        void setKeys(const byte_vector &encryptKey, const byte_vector &decryptKey);

        byte_vector getEncryptionKey() const;
        byte_vector getDecryptionKey() const;

        size_t getMaxPlainTextLength_() const override;
        size_t getFixedCipherTextLength_() const override;

    private:
        RSA::PrivateKey privateKey_;    // key used for decryption
        RSA::PublicKey publicKey_;      // key used for encryption

#if defined(CRYPTO_ALGORITHM_RSAES_PKCS1)
        RSAES_PKCS1v15_Encryptor encryptor_;
        RSAES_PKCS1v15_Decryptor decryptor_;
#elif defined(CRYPTO_ALGORITHM_RSAES_OAEP_SHA1)
        RSAES_OAEP_SHA_Encryptor encryptor_;
        RSAES_OAEP_SHA_Decryptor decryptor_;
#elif defined(CRYPTO_ALGORITHM_RSAES_OAEP_SHA256)
        RSAES<OAEP<SHA256>>::Encryptor encryptor_;
        RSAES<OAEP<SHA256>>::Decryptor decryptor_;
#endif
        std::size_t maxPlainTextLength_ = 0;
        std::size_t fixedCipherTextLength_ = 0;
        AutoSeededRandomPool randomGenerator_;

        mutable bool publicKeyInitialized_ = false;
        mutable bool privateKeyInitialized_ = false;

    private:
        void initCryptors();
        inline void testIfPublicKeyInitialized() const;
        inline void testIfPrivateKeyInitialized() const;
    };

    class RSAServerCrypto : public RSACrypto  {
    public:
        RSAServerCrypto(const std::string &publicKeyFilename = DEFAULT_PUBLIC_KEY_FILENAME,
            const std::string &privateKeyFilename = DEFAULT_PRIVATE_KEY_FILENAME);

        byte_vector encrypt(const byte_vector &text) override;
        byte_vector decrypt(const byte_vector &cipher) override;

        byte_vector getPublicKey() const;
        byte_vector getPrivateKey() const;

        size_t getMaxPlainTextLength_() const override;
        size_t getFixedCipherTextLength_() const override;

        //TODO (not implemented as of yet)
        void saveKeysToFiles(const std::string &publicKeyFilename = DEFAULT_PUBLIC_KEY_FILENAME,
            const std::string &privateKeyFilename = DEFAULT_PRIVATE_KEY_FILENAME) const;
        static void generateKeyFiles(const std::string &publicKeyFilename = DEFAULT_PUBLIC_KEY_FILENAME,
            const std::string &privateKeyFilename = DEFAULT_PRIVATE_KEY_FILENAME);


    private:
        crypto::RSASimpleCrypto rsaCrypto_;
        //std::string encryptKeyFilename;
        //std::string decryptKeyFilename;
        static const char* DEFAULT_PUBLIC_KEY_FILENAME;
        static const char* DEFAULT_PRIVATE_KEY_FILENAME;

    private:
        /*void saveKeysToFileSystem(const std::string &encryptKeyFilename, const std::string &decryptKeyFilename, const byte_vector &publicKeyData, const byte_vector &privateKeyData);
        bool loadKeysFromFileSystem(const std::string &encryptKeyFilename, const std::string &decryptKeyFilename, byte_vector &publicKeyDataTarget, byte_vector &privateKeyDataTarget);*/

        static byte_vector loadKeyFromFile(const std::string &keyFilename);
    };

    class AESCrypto {
    public:
        AESCrypto();
        AESCrypto(const byte_vector &symmetricKey);

        byte_vector encrypt(const byte_vector &text);
        byte_vector decrypt(const byte_vector &cipher);

        void setSymmetricKey(const byte_vector &symmetricKey);
        byte_vector getSymmetricKey() const;

    private:
        SecByteBlock key_;

        ECB_Mode<AES>::Encryption encryptor_;
        ECB_Mode<AES>::Decryption decryptor_;

        AutoSeededRandomPool randomGenerator_;

        //SecByteBlock iv_;
        //CFB_Mode<AES>::Encryption encryptor_;
        //CFB_Mode<AES>::Decryption decryptor_;

    private:
        void initCryptors();
    };
}

#endif /*CRYPTO_H*/