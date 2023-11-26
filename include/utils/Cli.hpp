#pragma once

#include <string>

class Cli {
    public:
        static void info(std::string message);
        static void warning(std::string message);
        static void debug(std::string message);

        static std::string formatInt(int number, size_t k);
};