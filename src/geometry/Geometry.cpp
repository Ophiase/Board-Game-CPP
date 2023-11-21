#include "geometry/Geometry.hpp"
#include "graphics/ResourcesLoader.hpp"

sf::Vector2f Geometry::toFloat (sf::Vector2u v) {
    return sf::Vector2f(
        static_cast<float>(v.x),
        static_cast<float>(v.y)
    );
}

sf::Vector2f Geometry::toFloat (sf::Vector2i v) {
    return sf::Vector2f(
        static_cast<float>(v.x),
        static_cast<float>(v.y)
    );
}

float Geometry::toRatio(sf::Vector2f v) { return v.x / v.y; }
float Geometry::toRatio(sf::Vector2i v) { return toRatio(toFloat(v)); }
float Geometry::toRatio(sf::Vector2u v) { return toRatio(toFloat(v)); }

float Geometry::toRatio(Texture::SourceTexture texture) {
    return toRatio(ResourcesLoader::getTexture(texture).getSize());
}