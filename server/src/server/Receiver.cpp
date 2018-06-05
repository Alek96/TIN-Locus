#include "Receiver.h"
#include "log/Logger.h"

using namespace Log;
using namespace message;
using namespace std::chrono_literals;

Receiver::Receiver() : BasicThread("Receiver") {
    ePollManager.setFlags(EPOLLIN | EPOLLET);
}
