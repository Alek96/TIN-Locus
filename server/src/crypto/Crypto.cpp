#include "Crypto.h"

#include <cryptopp/files.h>
#include <cryptopp/files.h>

#include <stdexcept>
#include <sstream>
#include <cstring>

using namespace CryptoPP;

namespace crypto {
    RSASimpleCrypto::RSASimpleCrypto() : publicKeyInitialized_(false), privateKeyInitialized_(false) {}

    RSASimpleCrypto::RSASimpleCrypto(const byte_vector &encryptKey, const byte_vector &decryptKey) {
        setEncryptionKey(encryptKey);
        setDecryptionKey(decryptKey);
    }

    byte_vector RSASimpleCrypto::encrypt(const byte_vector &text) {
        testIfPublicKeyInitialized();
        //NOTE: for RSA 512 (OAEP SHA) max text length is 22 (bytes)
        if (text.size() > maxPlainTextLength_) {
            std::ostringstream errorMsg;
            errorMsg << "Given plain text to encrypt with RSA exceeds max length, which is: " << maxPlainTextLength_;

            throw std::length_error(errorMsg.str());
        }

        std::string cipher; // encrypted text

        StringSource ess(
                text.data(), text.size(), true,
                new PK_EncryptorFilter(randomGenerator_, encryptor_,
                                       new StringSink(cipher)
                )
        );

        return byte_vector(cipher.begin(), cipher.end());
    }

    byte_vector RSASimpleCrypto::decrypt(const byte_vector &cipher) {
        testIfPrivateKeyInitialized();
        if (cipher.size() != fixedCipherTextLength_) {
            std::ostringstream errorMsg;
            errorMsg << "Given cipher text to decrypt with RSA doesn't max the required length, which is: "
                     << fixedCipherTextLength_;

            throw std::length_error(errorMsg.str());
        }

        std::string recovered; // decrypted text

        StringSource dss(
                cipher.data(), cipher.size(), true,
                new PK_DecryptorFilter(randomGenerator_, decryptor_,
                                       new StringSink(recovered)
                )
        );

        return byte_vector(recovered.begin(), recovered.end());
    }

    void RSASimpleCrypto::setEncryptionKey(const byte_vector &encryptKey) {
        StringSource encryptKeySource(encryptKey.data(), encryptKey.size(), true);
        ByteQueue queue;
        encryptKeySource.TransferTo(queue);
        queue.MessageEnd();
        publicKey_.Load(queue);

        if (!publicKey_.Validate(randomGenerator_, 2))
            throw std::runtime_error("RSA public key validation failed");

        publicKeyInitialized_ = true;
        initCryptors();
    }

    void RSASimpleCrypto::setDecryptionKey(const byte_vector &decryptKey) {
        StringSource decryptKeySource(decryptKey.data(), decryptKey.size(), true);
        ByteQueue queue;
        decryptKeySource.TransferTo(queue);
        queue.MessageEnd();
        privateKey_.Load(queue);

        if (!privateKey_.Validate(randomGenerator_, 2))
            throw std::runtime_error("RSA private key validation failed");

        privateKeyInitialized_ = true;
        initCryptors();
    }

    void RSASimpleCrypto::setKeys(const byte_vector &encryptKey, const byte_vector &decryptKey) {
        setEncryptionKey(encryptKey);
        setDecryptionKey(decryptKey);
    }

    byte_vector RSASimpleCrypto::getEncryptionKey() const {
        testIfPublicKeyInitialized();
        std::string keyString;

        ByteQueue queue;
        publicKey_.Save(queue);
        StringSink keySS(keyString);
        queue.CopyTo(keySS);
        keySS.MessageEnd();

        auto vecPublicKey = byte_vector(keyString.begin(), keyString.end());

        return vecPublicKey;
    }

    byte_vector RSASimpleCrypto::getDecryptionKey() const {
        testIfPrivateKeyInitialized();
        std::string keyString;

        ByteQueue queue;
        privateKey_.Save(queue);
        StringSink keySS(keyString);
        queue.CopyTo(keySS);
        keySS.MessageEnd();

        return byte_vector(keyString.begin(), keyString.end());
    }

    void RSASimpleCrypto::initCryptors() {
        if (publicKeyInitialized_) {
#if defined(CRYPTO_ALGORITHM_RSAES_PKCS1)
            RSAES_PKCS1v15_Encryptor encryptor(publicKey_);
#elif defined(CRYPTO_ALGORITHM_RSAES_OAEP_SHA1)
            RSAES_OAEP_SHA_Encryptor encryptor(publicKey_);
#elif defined(CRYPTO_ALGORITHM_RSAES_OAEP_SHA256)
            RSAES<OAEP<SHA256>>::Encryptor encryptor(publicKey_);
#endif
            encryptor_ = encryptor;
            maxPlainTextLength_ = encryptor_.FixedMaxPlaintextLength();
        }

        if (privateKeyInitialized_) {
#if defined(CRYPTO_ALGORITHM_RSAES_PKCS1)
            RSAES_PKCS1v15_Decryptor decryptor(privateKey_);
#elif defined(CRYPTO_ALGORITHM_RSAES_OAEP_SHA1)
            RSAES_OAEP_SHA_Decryptor decryptor(privateKey_);
#elif defined(CRYPTO_ALGORITHM_RSAES_OAEP_SHA256)
            RSAES<OAEP<SHA256>>::Decryptor decryptor(privateKey_);
#endif
            decryptor_ = decryptor;
            fixedCipherTextLength_ = decryptor_.FixedCiphertextLength();
        }
    }

    inline void RSASimpleCrypto::testIfPublicKeyInitialized() const {
        if (!publicKeyInitialized_)
            throw std::runtime_error("Encryption key is not initialized for RSASimpleCrypto object.");
    }

    inline void RSASimpleCrypto::testIfPrivateKeyInitialized() const {
        if (!privateKeyInitialized_)
            throw std::runtime_error("Decryption key is not initialized for RSASimpleCrypto object.");
    }

    size_t RSASimpleCrypto::getMaxPlainTextLength_() const {
        return maxPlainTextLength_;
    }

    size_t RSASimpleCrypto::getFixedCipherTextLength_() const {
        return fixedCipherTextLength_;
    }


    byte_vector RSAServerCrypto::loadKeyFromFile(const std::string &keyFilename) {
        std::string keyStr;
        FileSource file(keyFilename.c_str(), true,
                        new StringSink(keyStr));

        return byte_vector(keyStr.begin(), keyStr.end());
    }

    RSAServerCrypto::RSAServerCrypto(const std::string &publicKeyFilename, const std::string &privateKeyFilename) :
            rsaCrypto_(loadKeyFromFile(publicKeyFilename), loadKeyFromFile(privateKeyFilename)) {}

    byte_vector RSAServerCrypto::encrypt(const byte_vector &text) {
        return rsaCrypto_.encrypt(text);
    }

    byte_vector RSAServerCrypto::decrypt(const byte_vector &cipher) {
        return rsaCrypto_.decrypt(cipher);
    }

    byte_vector RSAServerCrypto::getPublicKey() const {
        return rsaCrypto_.getEncryptionKey();
    }

    byte_vector RSAServerCrypto::getPrivateKey() const {
        return rsaCrypto_.getDecryptionKey();
    }

    /*void RSAServerCrypto::saveKeysToFileSystem(const std::string &encryptKeyFilename, const std::string &decryptKeyFilename, const byte_vector &publicKeyData, const byte_vector &privateKeyData) {
        {
            StringSource keyToFile(publicKeyData.data(), publicKeyData.size(), true,
                new FileSink(encryptKeyFilename.c_str()));
        }
        {
            StringSource keyToFile(privateKeyData.data(), privateKeyData.size(), true,
                new FileSink(decryptKeyFilename.c_str()));
        }
    }

    bool RSAServerCrypto::loadKeysFromFileSystem(const std::string &encryptKeyFilename, const std::string &decryptKeyFilename, byte_vector &publicKeyDataTarget, byte_vector &privateKeyDataTarget) {
        try {
            {
                std::string publicKeyStr;
                FileSource file(encryptKeyFilename.c_str(), true,
                    new StringSink(publicKeyStr));

                publicKeyDataTarget.assign(publicKeyStr.begin(), publicKeyStr.end());
            }
            {
                std::string privateKeyStr;
                FileSource file(decryptKeyFilename.c_str(), true,
                    new StringSink(privateKeyStr));

                privateKeyDataTarget.assign(privateKeyStr.begin(), privateKeyStr.end());
            }
            return true;
            // load successful
        }
        catch (const FileSource::Err &err) {
            return false;
            // load unsuccessful
        }
    }*/

    const char *RSAServerCrypto::DEFAULT_PUBLIC_KEY_FILENAME = "rsa-public.key";
    const char *RSAServerCrypto::DEFAULT_PRIVATE_KEY_FILENAME = "rsa-private.key";

    size_t RSAServerCrypto::getMaxPlainTextLength_() const {
        return rsaCrypto_.getMaxPlainTextLength_();
    }

    size_t RSAServerCrypto::getFixedCipherTextLength_() const {
        return rsaCrypto_.getFixedCipherTextLength_();
    }

    /*struct SymmetricKeyAndIV {
        std::string key;
        std::string iv;
        // byte arrays?

        SymmetricKeyAndIV(const char *keyPtr, std::size_t keySize, const char *ivPtr, std::size_t ivSize) : key(keyPtr, keySize), iv(ivPtr, ivSize) {
        }
    };*/

    AESCrypto::AESCrypto() {
        // Generate a random key
        // Default key length is 16B (128b)
        key_.Assign((byte *) NULL, AES::DEFAULT_KEYLENGTH);
        randomGenerator_.GenerateBlock(key_, key_.size());

        // Generate a random IV (Initialization vector)
        // Block size for AES is always 16B
        //iv_.Assign(AES::BLOCKSIZE, (byte)0);
        //randomGenerator_.GenerateBlock(iv_, iv_.size());   

        initCryptors();
    }

    AESCrypto::AESCrypto(const byte_vector &symmetricKey) {
        if (symmetricKey.size() != AES::DEFAULT_KEYLENGTH) {
            std::ostringstream errorMsg;
            errorMsg << "Given symmetric key for AES doesn't max the required length, which is: "
                     << AES::DEFAULT_KEYLENGTH;

            throw std::length_error(errorMsg.str());
        }

        key_.Assign((const byte *) symmetricKey.data(), symmetricKey.size());

        initCryptors();
    }

    byte_vector AESCrypto::encrypt(const byte_vector &text) {
        std::string cipher;

        //NOTE: The StreamTransformationFilter adds padding
        //  as required. ECB and CBC Mode must be padded
        //  to the block size of the cipher.
        StringSource ess(
                text.data(), text.size(), true,
                new StreamTransformationFilter(encryptor_,
                                               new StringSink(cipher)
                )
        );

        return byte_vector(cipher.begin(), cipher.end());
    }

    byte_vector AESCrypto::decrypt(const byte_vector &cipher) {
        std::string recovered;

        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource dss(
                cipher.data(), cipher.size(), true,
                new StreamTransformationFilter(decryptor_,
                                               new StringSink(recovered)
                )
        );

        return byte_vector(recovered.begin(), recovered.end());
    }

    void AESCrypto::setSymmetricKey(const byte_vector &symmetricKey) {
        if (symmetricKey.size() != AES::DEFAULT_KEYLENGTH) {
            std::ostringstream errorMsg;
            errorMsg << "Given symmetric key for AES doesn't max the required length, which is: "
                     << AES::DEFAULT_KEYLENGTH;

            throw std::length_error(errorMsg.str());
        }

        key_.Assign((const byte *) symmetricKey.data(), symmetricKey.size());

        initCryptors();
    }

    /*SymmetricKeyAndIV getSymmetricKey() const {
        return SymmetricKeyAndIV(
            key_.data(), key_.SizeInBytes(),
            iv_.data(), iv_.SizeInBytes());
    }*/

    byte_vector AESCrypto::getSymmetricKey() const {
        return byte_vector((unsigned char *) key_.data(), (unsigned char *) key_.data() + key_.SizeInBytes());
    }

    void AESCrypto::initCryptors() {
        encryptor_.SetKey(key_, key_.size());
        decryptor_.SetKey(key_, key_.size());
    }
}