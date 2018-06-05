#ifndef LOCUS_LOGGER_H
#define LOCUS_LOGGER_H

#include <iostream>
#include <mutex>

namespace Log {

    class Logger {
        Logger() = default;

    public:
        Logger(Logger const &) = delete;

        void operator=(Logger const &) = delete;

        static Logger &getInstance() {
            static Logger instance;
            return instance;
        }

        void logMessage(std::string message);
        void logError(std::string error);
        void logDebug(std::string debug);

        void setOutputStream(std::ostream *output);
        void setShouldShowDebug(bool shouldShowDebug);

    private:
        std::ostream *output = &std::cerr;
        bool shouldShowDebug = true;
        std::mutex loggerMutex;

        std::string getTime();
    };

} // end Log namespace


#endif //LOCUS_LOGGER_H
