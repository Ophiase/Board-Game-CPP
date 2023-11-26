#include "graphics/screen/Canvas.hpp"
#include "geometry/Geometry.hpp"

Canvas::Canvas(Launcher *launcher, 
    sf::Vector2f position, sf::Vector2f size
    ) : Container{launcher} {

    this->setPosition(position);
    this->setSize(size);
}

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