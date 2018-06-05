#include "Logger.h"

#include <chrono>
#include "Colour.h"

namespace Log {

    void Logger::logMessage(std::string message) {
        std::lock_guard<std::mutex> guard(loggerMutex);

        *output << "LOG: " << getTime() << "  " << message << std::endl;
    }

    void Logger::logError(std::string error) {
        std::lock_guard<std::mutex> guard(loggerMutex);

        *output << Colour(Colour::Error)
                << "ERROR: " << getTime() << "  " << error
                << Colour(Colour::White) << std::endl;
    }

    void Logger::logDebug(std::string debug) {
        if (!shouldShowDebug)
            return;

        std::lock_guard<std::mutex> guard(loggerMutex);

        *output << Colour(Colour::Warning)
                << "DEBUG: " << getTime() << "  " << debug
                << Colour(Colour::White) << std::endl;
    }

    void Logger::setShouldShowDebug(bool shouldShowDebug) {
        Logger::shouldShowDebug = shouldShowDebug;
    }

    void Logger::setOutputStream(std::ostream *output) {
        std::lock_guard<std::mutex> guard(loggerMutex);
        Logger::output = output;
    }

    std::string Logger::getTime() {
        auto time = std::chrono::system_clock::now();
        std::time_t timeT = std::chrono::system_clock::to_time_t(time);
        return std::string(std::ctime(&timeT)).substr(0, std::string(std::ctime(&timeT)).size() - 1);
    }

} // end Log namespace