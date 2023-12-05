#include "utils/Cli.hpp"
#include <iostream>

bool Cli::muted = false;

void Cli::info(std::string message) {
    if (muted) return;
    std::cout << "[Info] " << message << std::endl;
}

void Cli::warning(std::string message) {
    if (muted) return;
    std::cout << "<Warning> " << message << std::endl;
}

void Cli::error(std::string message) {
    std::cerr << "<Error> " << message << std::endl;
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

std::string Cli::toString(sf::FloatRect rect) {
    return "[" +
        std::to_string(rect.left) + ", " +
        std::to_string(rect.top) + " : " +
        std::to_string(rect.width) + ", " +
        std::to_string(rect.height) + "]";
}

std::string Cli::toString(sf::Vector2f vect) {
    return "(" +
        std::to_string(vect.x) + ", " +
        std::to_string(vect.y) + ")";
}

std::string Cli::toString(sf::Vector2i vect) {
    return "(" +
        std::to_string(vect.x) + ", " +
        std::to_string(vect.y) + ")";
}


std::string Cli::toString(std::vector<int> vect) {
    std::string result = "[ ";

    for (size_t i = 0; i < vect.size() - 1; ++i)
        result += std::to_string(vect[i]) + ", ";
    result += std::to_string(vect[vect.size() -1]);
        
    result += " ]";
    return result;
}

std::string Cli::toString(std::vector<sf::Vector2f> vect) {
    std::string result = "[ ";

    for (size_t i = 0; i < vect.size() - 1; ++i)
        result += toString(vect[i]) + ", ";
    result += toString(vect[vect.size()-1]);

    result += " ]";
    return result;
}

std::string Cli::toString(std::vector<sf::Vector2i> vect) {
    std::string result = "[ ";

    for (size_t i = 0; i < vect.size() -1; ++i)
        result += toString(vect[i]) + ", ";
    result += toString(vect[vect.size()-1]);

    result += " ]";
    return result;
}
