#ifndef LOCUS_ENCRYPTCLIENT_H
#define LOCUS_ENCRYPTCLIENT_H

#include <memory>
#include "DecoratorClient.h"

class EncryptClient : public DecoratorClient {
public:
    explicit EncryptClient(std::unique_ptr<Client> &&client);

    EncryptClient(EncryptClient&&) noexcept = default;

    ~EncryptClient() override = default;

    void recv() override;
    void send() override {}

    void sendData(const std::vector<unsigned char> &bytes) override;
    std::vector<unsigned char> recvData() override;
};


#endif //LOCUS_ENCRYPTCLIENT_H
