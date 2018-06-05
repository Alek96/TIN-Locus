#include <iostream>
#include "Crypto.h"

inline crypto::byte_vector toByteVector(const std::string &string) {
    return crypto::byte_vector(string.begin(), string.end());
}
inline std::string toString(const crypto::byte_vector &vector) {
    return std::string(vector.begin(), vector.end());
}

int main() {
    crypto::RSAServerCrypto rsaServerCrypto;
    crypto::AESCrypto aesCrypto;
    std::string res = "Ala ma kota";
    crypto::byte_vector vec = toByteVector(res);

    //NOTE: for RSA 512 (OAEP SHA) max text length is 22 (bytes)
    std::cout << "RSA Before encryption: " << res << '\n';
    std::cout << "RSA Text length: " << res.size() << '\n';

    vec = rsaServerCrypto.encrypt(vec);
    res = toString(vec);

    std::cout << "RSA Encrypted: " << res << '\n';
    std::cout << "RSA Encrypted length: " << res.size() << '\n';

    vec = rsaServerCrypto.decrypt(vec);
    res = toString(vec);

    std::cout << "RSA Decrypted: " << res << '\n';

    res = "Ala ma kota";
    vec = toByteVector(res);

    std::cout << "AES Before encryption: " << res << '\n';
    std::cout << "AES Text length: " << res.size() << '\n';

    vec = aesCrypto.encrypt(vec);
    res = toString(vec);

    //NOTE: Cipher after AES encryption (ECB mode) is padded to the block size (16B)
    std::cout << "AES Encrypted: " << res << '\n';
    std::cout << "AES Encrypted length: " << res.size() << '\n';

    vec = aesCrypto.decrypt(vec);
    res = toString(vec);

    std::cout << "AES Decrypted: " << res << '\n';

    // cipher with 2 blocks length:
    res = "Ala ma kota i duuuuzego psa";
    vec = toByteVector(res);

    std::cout << "AES long Before encryption: " << res << '\n';
    std::cout << "AES long Text length: " << res.size() << '\n';

    vec = aesCrypto.encrypt(vec);
    res = toString(vec);

    //NOTE: Cipher after AES encryption (ECB mode) is padded to the block size (16B)
    std::cout << "AES long Encrypted: " << res << '\n';
    std::cout << "AES long Encrypted length: " << res.size() << '\n';

    vec = aesCrypto.decrypt(vec);
    res = toString(vec);

    std::cout << "AES long Decrypted: " << res << '\n';
    return 0;
}