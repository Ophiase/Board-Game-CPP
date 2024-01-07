#include "utils/Cli.hpp"
#include <iostream>

bool Cli::muted = false;

void Cli::info(std::string message) {
    if (muted) return;
    std::cout << "\x1B[32m[Info]\x1B[0m " 
        << message << std::endl;
}

void Cli::warning(std::string message) {
    if (muted) return;
    std::cout << "\x1B[33m<Warning>\x1B[0m "
        << message << std::endl;
}

void Cli::error(std::string message) {
    std::cerr << "\x1B[31m<Error>\x1B[0m " 
        << message << std::endl;
}

void Cli::debug(std::string message) {
    #ifdef DEBUG
        std::cerr << "\x1B[36m<Debug>\x1B[0m "
            << message << std::endl;
    #else
        (void)message;
    #endif
}

std::string Cli::formatInt(int number, size_t k) {
    std::string numberStr = std::to_string(number);
    size_t zerosToAdd = (k > numberStr.length()) ? k - numberStr.length() : 0;
    return std::string(zerosToAdd, '0') + numberStr;
}

std::string Cli::separation(int n) {
    if (n == 15)
        return "---------------";
    
    // todo use memset over an array instead
    std::string s = ""; 
    for (int i = 0; i < n; i++)
        s += '-';
    return s;
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

std::string Cli::toString(SidePosition vect) {
    return "(" +
        std::to_string(vect.sideVector.x) + ", " +
        std::to_string(vect.sideVector.y) + ")";
}


std::string Cli::toString(std::vector<int> vect) {
    if (vect.size() == 0)
        return "[empty]";
        
    std::string result = "[ ";

    for (size_t i = 0; i < vect.size() - 1; ++i)
        result += std::to_string(vect[i]) + ", ";
    result += std::to_string(vect[vect.size() -1]);
        
    result += " ]";
    return result;
}

std::string Cli::toString(std::vector<sf::Vector2f> vect) {
    if (vect.size() == 0)
        return "[empty]";

    std::string result = "[ ";

    for (size_t i = 0; i < vect.size() - 1; ++i)
        result += toString(vect[i]) + ", ";
    result += toString(vect[vect.size()-1]);

    result += " ]";
    return result;
}

std::string Cli::toString(std::vector<sf::Vector2i> vect) {
    if (vect.size() == 0)
        return "[empty]";

    std::string result = "[ ";

    for (size_t i = 0; i < vect.size() -1; ++i)
        result += toString(vect[i]) + ", ";
    result += toString(vect[vect.size()-1]);

    result += " ]";
    return result;
}

std::string Cli::toString(std::vector<SidePosition> vect) {
    if (vect.size() == 0)
        return "[empty]";

    std::string result = "[ ";

    for (size_t i = 0; i < vect.size() -1; ++i)
        result += toString(vect[i]) + ", ";
    result += toString(vect[vect.size()-1]);

    result += " ]";
    return result;
}
