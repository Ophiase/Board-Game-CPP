#include "geometry/Geometry.hpp"
#include "graphics/ResourcesLoader.hpp"

// ------------------------------------------

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

// ------------------------------------------

sf::FloatRect Geometry::fit(sf::Vector2f resolution, 
        sf::Vector2f targetShapePosition,
        sf::Vector2f targetShapeSize
    ) {
    
    float targetRatio = toRatio(targetShapeSize);
    float resolutionRatio = toRatio(resolution);

    sf::FloatRect shape {targetShapePosition, targetShapeSize};

    if (resolutionRatio > targetRatio) {
        shape.width = targetShapeSize.x * (resolutionRatio/targetRatio);
        shape.left -= (shape.width - targetShapeSize.x) / 2.0;
    } else {
        shape.height = targetShapeSize.y * (targetRatio/resolutionRatio);
        shape.top -= (shape.height - targetShapeSize.y) / 2.0;
    }

    return shape;
}

sf::FloatRect Geometry::fit(sf::Vector2f resolution, sf::FloatRect targetShape) {
    return fit(
        resolution,
        sf::Vector2f(targetShape.left, targetShape.top),
        sf::Vector2f(targetShape.width, targetShape.height)
        );
}

// ------------------------------------------

float Geometry::toRatio(sf::Vector2f v) { return v.x / v.y; }
float Geometry::toRatio(sf::Vector2i v) { return toRatio(toFloat(v)); }
float Geometry::toRatio(sf::Vector2u v) { return toRatio(toFloat(v)); }

float Geometry::toRatio(Texture::SourceTexture texture) {
    return toRatio(ResourcesLoader::getTexture(texture).getSize());
}

// ------------------------------------------

sf::Vector2f Geometry::floatRectPosition(sf::FloatRect rect) {
    return sf::Vector2f{rect.left, rect.top};
}

sf::Vector2f Geometry::floatRectSize(sf::FloatRect rect) {
    return sf::Vector2f{rect.width, rect.height};
}
        
sf::FloatRect Geometry::rectangleShapeToFloatRect(sf::RectangleShape shape) {
    return sf::FloatRect{shape.getPosition(), shape.getSize()};
}

void Geometry::applyFloatRectToRectangleShape(
    sf::RectangleShape & shape, sf::FloatRect const rect
    ) {
        
    shape.setPosition(floatRectPosition(rect));
    shape.setSize(floatRectSize(rect));
}

sf::FloatRect Geometry::scaleRect(sf::FloatRect rect, float factor) {
    sf::Vector2f mid {
        rect.left + (rect.width / 2.0f),
        rect.top + (rect.height / 2.0f)
    };

    return scaleRect(rect, factor, mid);
}

sf::FloatRect Geometry::scaleRect(sf::FloatRect rect, float factor, sf::Vector2f origin) {
    return sf::FloatRect{
        factor * (rect.left - origin.x) + origin.x,
        factor * (rect.top - origin.y) + origin.y,
        rect.width * factor,
        rect.height * factor
    };
}

sf::Vector2f Geometry::spaceTransform(
    sf::Vector2f vector, sf::FloatRect oldSpace, sf::FloatRect newSpace
    ) {
    
    vector += floatRectPosition(oldSpace);
    vector.x *= floatRectSize(oldSpace).x;
    vector.y *= floatRectSize(oldSpace).y;

    vector.x /= floatRectSize(newSpace).x;
    vector.y /= floatRectSize(newSpace).y;
    vector -= floatRectPosition(newSpace);

    return vector;
}