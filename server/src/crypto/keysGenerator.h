#ifndef LOCUS_KEYSGENERATOR_H
#define LOCUS_KEYSGENERATOR_H

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>

#include <stdexcept>

namespace crypto {
    void keysGenerator(unsigned int length);
}

#endif //LOCUS_KEYSGENERATOR_H
