#pragma once

#include <SFML/Graphics.hpp>
#include "graphics/ResourcesLoader.hpp"

class Geometry {
    public:
        static sf::FloatRect fit();

        static sf::Vector2f toFloat(sf::Vector2u v);
        static sf::Vector2f toFloat(sf::Vector2i v);

        static float toRatio(sf::Vector2f v);
        static float toRatio(sf::Vector2u v);
        static float toRatio(sf::Vector2i v);
        static float toRatio(Texture::SourceTexture texture);
};