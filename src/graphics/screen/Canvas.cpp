#include "graphics/screen/Canvas.hpp"
#include "geometry/Geometry.hpp"

Canvas::Canvas(Container *parent,
    sf::Vector2f position, sf::Vector2f size,
    bool withHandler
    ) : Container{parent, withHandler} {

    this->setPosition(position);
    this->setSize(size);
}

void Canvas::setSize(sf::Vector2f size) {
    this->size = size;
}

sf::Vector2f Canvas::getSize() const {
    return this->size;
}

void Canvas::setPosition(sf::Vector2f position) {
    this->position = position;
}

sf::Vector2f Canvas::getPosition() const {
    return this->position;
}

sf::Vector2f Canvas::getRelMid() const {
    return {
        this->getSize().x * 0.5f,
        this->getSize().y * 0.5f
    };
}

sf::Vector2f Canvas::getMid() const {
    return this->getPosition() + this->getRelMid();
}

sf::Vector2f Canvas::getEndCorner() const {
    return this->getPosition() + this->getSize();
}

void Canvas::center(sf::Vector2f position) {
    this->setPosition(position - this->getRelMid());
}

void Canvas::center(sf::Vector2f position, sf::Vector2f relAnchor) {
    this->position = position - relAnchor;
}


sf::FloatRect Canvas::getFloatRect() const {
    return sf::FloatRect{this->position, this->size};
}

void Canvas::setFloatRect(sf::FloatRect rect) {
    Geometry::applyFloatRect(this->position, this->size, rect);
}


void Canvas::fit(sf::FloatRect targetBbox) {
    this->setFloatRect(
        Geometry::fitInside(
            this->getSize(), targetBbox
        )
    );
}

sf::Vector2f Canvas::getRelativeMousePosition() const {
    auto mousePosition = Geometry::toFloat(sf::Mouse::getPosition());
    auto screenSpace = sf::FloatRect{
        0.0, 0.0, 
        (float)this->getConstRenderWindow().getSize().x, 
        (float)this->getConstRenderWindow().getSize().y
        };
    auto buttonSpace = sf::FloatRect{
        this->getPosition(), this->getSize()
    };
    
    return Geometry::spaceTransform(
        mousePosition, screenSpace, buttonSpace
        );
}