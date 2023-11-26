#include "utils/Cli.hpp"
#include <iostream>

#define UNREFERENCED_PARAMETER(P)          (P)

void Cli::info(std::string message) {
    std::cout << "[Info] " << message << std::endl;
}

void Cli::warning(std::string message) {
    std::cerr << "<Warning> " << message << std::endl;
}

void Cli::debug(std::string message) {
    #ifdef DEBUG
        std::cerr << "<Debug> " << message << std::endl;
    #else
        (void)message;
    #endif
}

std::string Cli::formatInt(int number, size_t k) {
    std::string numberStr = std::to_string(number);
    size_t zerosToAdd = (k > numberStr.length()) ? k - numberStr.length() : 0;
    return std::string(zerosToAdd, '0') + numberStr;
}
