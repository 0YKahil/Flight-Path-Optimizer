#pragma once

#include <string>

class Logger {
    public:
        enum class Level{
            Info,
            Warning,
            Error,
            Debug
        };

    
        static void info(const std::string& message);
        static void warning(const std::string& message);
        static void error(const std::string& message);
        static void debug(const std::string& message);

        static void log(Level level, const std::string& message);
};