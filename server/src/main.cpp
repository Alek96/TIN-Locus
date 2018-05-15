#include <iostream>
#include <thread>
#include "server/Receiver.h"
#include "server/Sender.h"
#include "server/Server.h"
#include "server/Acceptor.h"
#include "server/ClientManager.h"

using namespace message;

const uint16_t PORT = 5050;

int main() {
    Server server;
    std::thread serverThread([&server] { server(); });

    Receiver receiver;
    std::thread receiverThread([&receiver] { receiver(); });

    Sender sender;
    std::thread senderThread([&sender] { sender(); });

    ClientManager clientManager(server, receiver, sender);
    {
        Message msg(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(&clientManager);
        server.getBlockingQueue().push(std::move(msg));
    }

    Acceptor acceptor(PORT, &clientManager.getBlockingQueue());
    {
        Message msg(Message::AddEPollEvent);
        msg.ePollEvent = std::make_unique<EPollEvent *>(&acceptor);
        receiver.getBlockingQueue().push(std::move(msg));
    }


    std::cin.get();
    {
        Message msg = Message(Message::Close);
        sender.getBlockingQueue().push(std::move(msg));
        senderThread.join();
    }
    {
        Message msg = Message(Message::Close);
        receiver.getBlockingQueue().push(std::move(msg));
        receiverThread.join();
    }
    {
        Message msg = Message(Message::Close);
        server.getBlockingQueue().push(std::move(msg));
        serverThread.join();
    }

    return 0;
}

/*
 * odbierac tylko tyle danych ile pakiet - 1 bajt rozmiar pakietu, nie wiecej
 * parsowanie dopiero bo dostaniu calego pakietu
 * wysylanie ok
 * dodac pipa do BlockingQueue,
 * dodac watek klienta, czekajacy na epoll'u
 *
 * */