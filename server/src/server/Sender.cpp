#include "Sender.h"
#include "log/Logger.h"

using namespace Log;
using namespace message;
using namespace std::chrono_literals;

Sender::Sender() : BasicThread("Sender") {
    ePollManager.setFlags(EPOLLOUT | EPOLLET);
}
