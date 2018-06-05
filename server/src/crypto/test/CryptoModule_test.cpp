#include "gtest/gtest.h"
#include "crypto/CryptoModule.h"
#include "crypto/keysGenerator.h"

#define PRINT2

inline crypto::byte_vector toByteVector(const std::string &string) {
    return crypto::byte_vector(string.begin(), string.end());
}
inline std::string toString(const crypto::byte_vector &vector) {
    return std::string(vector.begin(), vector.end());
}

using namespace crypto;

TEST(CryptoModule_use, None) {
    CryptoModule cryptoModule;
    cryptoModule.use(CryptoModule::Algorithm::None);
    std::string res = "Ala ma kota";
    crypto::byte_vector vec = toByteVector(res);

    auto encryptVec = cryptoModule.encrypt(vec);
    auto decryptVec = cryptoModule.decrypt(encryptVec);

    EXPECT_EQ(vec, encryptVec);
    EXPECT_EQ(vec, decryptVec);
}

TEST(CryptoModule_use, ServerRSA) {
    CryptoModule cryptoModule;
    cryptoModule.use(CryptoModule::Algorithm::ServerRSA);
    std::string res = "Ala ma kota";
    crypto::byte_vector vec = toByteVector(res);

    auto encryptVec = cryptoModule.encrypt(vec);
    auto decryptVec = cryptoModule.decrypt(encryptVec);

    EXPECT_NE(vec, encryptVec);
    EXPECT_EQ(vec, decryptVec);
}


TEST(CryptoModule_use, ServerRSALongData) {
    CryptoModule cryptoModule;
    cryptoModule.use(CryptoModule::Algorithm::ServerRSA);
    std::string res = "Ala ma kota .......2.........3.........4....";
    crypto::byte_vector vec = toByteVector(res);

    auto encryptVec = cryptoModule.encrypt(vec);
    auto decryptVec = cryptoModule.decrypt(encryptVec);

#ifdef PRINT
    auto encryptRes = toString(encryptVec);
    auto decryptRes = toString(decryptVec);

    //NOTE: for RSA 512 (OAEP SHA) max text length is 22 (bytes)
    std::cout << "RSA Before encryption: " << res << '\n';
    std::cout << "RSA Text length: " << res.size() << '\n';

    std::cout << "RSA Encrypted: " << encryptRes << '\n';
    std::cout << "RSA Encrypted length: " << encryptRes.size() << '\n';

    std::cout << "RSA Decrypted: " << decryptRes << '\n';

    EXPECT_NE(res, encryptRes);
    EXPECT_EQ(res, decryptRes);
#endif
    
    EXPECT_NE(vec, encryptVec);
    EXPECT_EQ(vec, decryptVec);
}

TEST(CryptoModule_use, AES) {
    CryptoModule cryptoModule;
    cryptoModule.use(CryptoModule::Algorithm::AES);
    std::string res = "Ala ma kota";
    crypto::byte_vector vec = toByteVector(res);

    auto encryptVec = cryptoModule.encrypt(vec);
    auto decryptVec = cryptoModule.decrypt(encryptVec);

    EXPECT_NE(vec, encryptVec);
    EXPECT_EQ(vec, decryptVec);
}
