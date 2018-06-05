#include "Server.h"
#include "log/Logger.h"

using namespace Log;
using namespace message;
using namespace std::chrono_literals;

Server::Server() : BasicThread("Server") {
    ePollManager.setFlags(EPOLLIN | EPOLLET);
}