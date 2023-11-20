#pragma once

#include <SFML/Graphics.hpp>

class Geometry {
    public:
        static sf::FloatRect fit();
        static sf::Vector2f toFloat(sf::Vector2u v);
};