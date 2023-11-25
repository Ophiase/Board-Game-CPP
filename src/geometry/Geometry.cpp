#include "geometry/Geometry.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "utils/NotImplemented.hpp"
#include "stdexcept"

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

sf::FloatRect Geometry::fitOutside(sf::Vector2f resolution, 
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

sf::FloatRect Geometry::fitOutside(sf::Vector2f resolution, sf::FloatRect targetShape) {
    return fitOutside(
        resolution,
        sf::Vector2f(targetShape.left, targetShape.top),
        sf::Vector2f(targetShape.width, targetShape.height)
        );
}

sf::FloatRect Geometry::fitInside(sf::Vector2f resolution, 
        sf::Vector2f targetShapePosition,
        sf::Vector2f targetShapeSize
    ) {
    
    float targetRatio = toRatio(targetShapeSize);
    float resolutionRatio = toRatio(resolution);

    sf::FloatRect shape {targetShapePosition, targetShapeSize};

    if (resolutionRatio > targetRatio) {
        shape.height = targetShapeSize.y * (resolutionRatio/targetRatio);
        shape.top += (targetShapeSize.y - shape.height) / 2.0;
    } else {
        shape.width = targetShapeSize.x * (targetRatio/resolutionRatio);
        shape.left += (targetShapeSize.x - shape.width) / 2.0;
    }

    return shape;
}

sf::FloatRect Geometry::fitInside(sf::Vector2f resolution, sf::FloatRect targetShape) {
    return fitOutside(
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
    return toRatio(ResourcesLoader::getTexture(texture)->getSize());
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

    vector.x = ((vector.x + oldSpace.left) * oldSpace.width / newSpace.width)  - newSpace.left;
    vector.y = ((vector.y + oldSpace.top ) * oldSpace.height/ newSpace.height) - newSpace.top;

    return vector;
}

sf::FloatRect Geometry::spaceTransform(
    sf::FloatRect rect, sf::FloatRect oldSpace, sf::FloatRect newSpace
    ) {
    
    float xRatio = oldSpace.width / newSpace.width;
    float yRatio = oldSpace.width / newSpace.width;
    
    rect.left = ((rect.left + oldSpace.left) * xRatio) - newSpace.left;
    rect.top  = ((rect.top  + oldSpace.top ) * yRatio) - newSpace.top;

    rect.width  *= xRatio;
    rect.height *= yRatio;

    return rect;
}

bool Geometry::insideUnitBox(sf::Vector2f v) {
    return v.x >= 0.0 && v.y >= 0.0 && v.x <= 1.0 && v.y <= 1.0;
}

sf::FloatRect Geometry::minRectangle(sf::FloatRect a, sf::FloatRect b) {
    if (b.top > a.top)
        std::swap(a, b);

    sf::FloatRect result;
    if (a.left < b.left)
        result = sf::FloatRect{
            b.left, b.top,
            a.left + a.width - b.left, a.top + a.height - b.top
        };
    else
        result = sf::FloatRect{
            a.left, b.top,
            b.left + b.width - a.left, a.top + a.height - b.top
        };

    if (result.width < 0 || result.height < 0)
        throw std::invalid_argument("Non crossing rectangles");

    return result;
}

sf::FloatRect Geometry::minRectangle(
    sf::RectangleShape  a, sf::RectangleShape b
    ) {
    
    return minRectangle(rectangleShapeToFloatRect(a), rectangleShapeToFloatRect(b));
}