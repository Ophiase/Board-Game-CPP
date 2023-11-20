#include "geometry/Geometry.hpp"

sf::Vector2f Geometry::toFloat (sf::Vector2u v) {
    return sf::Vector2f(
        static_cast<float>(v.x),
        static_cast<float>(v.y)
    );
}