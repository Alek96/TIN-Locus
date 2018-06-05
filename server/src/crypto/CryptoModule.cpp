#include "CryptoModule.h"
#include <cmath>

namespace crypto {

    void CryptoModule::setOuterRSAKey(const byte_vector &key) {
        rsaSimpleCrypto.setEncryptionKey(key);
//        rsaSimpleCrypto.setDecryptionKey(key);
        outerRSAKey = true;
    }

    const byte_vector CryptoModule::getOuterRSAKey() const {
        return rsaSimpleCrypto.getEncryptionKey();
    }

    byte_vector CryptoModule::encryptRSA(RSACrypto *rsaCrypto, const byte_vector &text) {
        byte_vector encryptBytes;
        for (int i = 0; i < text.size(); i += rsaCrypto->getMaxPlainTextLength_()) {
            auto newBytes = rsaCrypto->encrypt(
                    byte_vector(text.begin() + i,
                                text.begin() + std::min(text.size(), i + rsaCrypto->getMaxPlainTextLength_())));
            encryptBytes.insert(encryptBytes.end(), newBytes.begin(), newBytes.end());
        }

        return encryptBytes;
    }

    byte_vector CryptoModule::decryptRSA(RSACrypto *rsaCrypto, const byte_vector &cipher) {
        byte_vector decryptBytes;
        for (int i = 0; i < cipher.size(); i += rsaCrypto->getFixedCipherTextLength_()) {
            auto newBytes = rsaCrypto->decrypt(
                    byte_vector(cipher.begin() + i,
                                cipher.begin() + std::min(cipher.size(), i + rsaCrypto->getFixedCipherTextLength_())));
            decryptBytes.insert(decryptBytes.end(), newBytes.begin(), newBytes.end());
        }

        return decryptBytes;
    }

    byte_vector CryptoModule::getSymmetricKey() const {
        return aesCrypto.getSymmetricKey();
    }

    void CryptoModule::use(const CryptoModule::Algorithm &algo) {
        algorithm = algo;
    }

    byte_vector CryptoModule::encrypt(const byte_vector &text) {
        switch (algorithm) {
            case Algorithm::None:
                return text;
            case Algorithm::ServerRSA:
                return encryptRSA(&rsaServerCrypto, text);
            case Algorithm::OuterRSA:
                return encryptRSA(&rsaSimpleCrypto, text);
            case Algorithm::AES:
                return aesCrypto.encrypt(text);
        }
    }

    byte_vector CryptoModule::decrypt(const byte_vector &cipher) {
        switch (algorithm) {
            case Algorithm::None:
                return cipher;
            case Algorithm::ServerRSA:
                return decryptRSA(&rsaServerCrypto, cipher);
            case Algorithm::OuterRSA:
                return decryptRSA(&rsaSimpleCrypto, cipher);
            case Algorithm::AES:
                return aesCrypto.decrypt(cipher);
        }
    }

}