#pragma once

#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
#include "engine/type/SidePath.hpp"

class Cli {
    public:
        static bool muted;

        static void info(std::string message);
        static void warning(std::string message);
        static void error(std::string message);
        static void debug(std::string message);

        static std::string formatInt(int number, size_t k);
        static std::string separation(int n = 15);
        
        static std::string toString(sf::FloatRect rect);
        static std::string toString(sf::Vector2f vect);
        static std::string toString(sf::Vector2i vect);
        static std::string toString(SidePosition vect);

        static std::string toString(std::vector<int> vect);
        static std::string toString(std::vector<float> vect);
        static std::string toString(std::vector<sf::Vector2f> vect);
        static std::string toString(std::vector<sf::Vector2i> vect);
        static std::string toString(std::vector<SidePosition> vect);

        template <typename T>
        static std::string toString(const T *pointer) {
            std::ostringstream oss;
            oss << "[ " <<  pointer << " ]";
            return oss.str();
        }
};