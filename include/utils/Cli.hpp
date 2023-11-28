#pragma once

#include <string>
#include "SFML/Graphics.hpp"

class Cli {
    public:
        static void info(std::string message);
        static void warning(std::string message);
        static void debug(std::string message);

        static std::string formatInt(int number, size_t k);
        
        static std::string to_string(sf::FloatRect rect);
        static std::string to_string(sf::Vector2f vect);
};