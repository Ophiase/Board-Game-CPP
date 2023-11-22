#include "graphics/screen/Button.hpp"
#include "geometry/Geometry.hpp"

void Button::handleEvent(sf::Event event) {
    auto mousePosition = getRelativeMousePosition();

    if (!Geometry::insideUnitBox(mousePosition)) 
        return;
    
    this->onHoover(event);
    if (event.type == sf::Event::MouseButtonPressed) 
        this->onClick(event);
}

sf::Vector2f Button::getRelativeMousePosition() const {
    auto mousePosition = Geometry::toFloat(sf::Mouse::getPosition());
    auto screenSpace = sf::FloatRect{
        0.0, 0.0, 
        (float)this->window.getSize().x, 
        (float)this->window.getSize().y
        };
    auto buttonSpace = sf::FloatRect{
        this->position, this->size
    };
    
    return Geometry::spaceTransform(
        mousePosition, screenSpace, buttonSpace
        );
}