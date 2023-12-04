#include "graphics/screen/Canvas.hpp"
#include "geometry/Geometry.hpp"
#include "utils/Cli.hpp"
#include "graphics/Launcher.hpp"

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

void Canvas::setSizeX(float sx) {
    auto oldSize = this->getSize();
    this->setSize(sf::Vector2f{
        sx,
        oldSize.y * sx/oldSize.x
    });
}

void Canvas::setSizeY(float sy) {
    auto oldSize = this->getSize();
    this->setSize(sf::Vector2f{
        oldSize.x * sy/oldSize.y,
        sy
    });
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
    return sf::FloatRect{
        this->getPosition(),
        this->getSize()
    };
}

void Canvas::setFloatRect(sf::FloatRect rect) {
    this->setPosition(sf::Vector2f(rect.left, rect.top));
    this->setSize(sf::Vector2f(rect.width, rect.height));
}


void Canvas::fit(sf::FloatRect targetBbox) {
    this->setFloatRect(
        Geometry::fitInside(
            this->getSize(), targetBbox
        )
    );
}

sf::Vector2f Canvas::getRelativeMousePosition() const {
    auto worldPosition = this->mouseWorldSpace();
    return sf::Vector2f{
        (worldPosition.x - this->getPosition().x) / this->getSize().x,
        (worldPosition.y - this->getPosition().y) / this->getSize().y
    };
}