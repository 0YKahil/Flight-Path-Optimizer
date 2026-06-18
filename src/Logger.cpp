#include "Logger.h"
#include <iostream>
#include <mutex>

namespace {
    std::mutex logMutex;

    std::ostream& streamForLevel(Logger::Level level) {
        if (level == Logger::Level::Warning || level == Logger::Level::Error) {
            return std::cerr;
        }

        return std::cout;
    }
}

void Logger::info(const std::string& message) {
    log(Level::Info, message);
}

void Logger::warning(const std::string& message) {
    log(Level::Warning, message);
}

void Logger::error(const std::string& message) {
    log(Level::Error, message);
}

void Logger::debug(const std::string& message) {
    log(Level::Debug, message);
}

void Logger::log(Level level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::string levelStr;

    switch (level) {
        case Level::Info:
            levelStr = "INFO";
            break;
        case Level::Warning:
            levelStr = "WARNING";
            break;
        case Level::Error:
            levelStr = "ERROR";
            break;
        case Level::Debug:
            levelStr = "DEBUG";
            break;
    }

    streamForLevel(level) << "[" << levelStr << "] " << message << std::endl;
}
