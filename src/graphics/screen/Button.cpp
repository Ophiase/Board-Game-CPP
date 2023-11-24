#include "graphics/screen/Button.hpp"
#include "geometry/Geometry.hpp"

Button::Button(
    Container *container, sf::Vector2f position, sf::Vector2f size) : 
    DrawableHandler{container->getLauncher()}, 
    position{position}, 
    size{size} {

    container->add(this);
}

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
        (float)this->getConstRenderWindow().getSize().x, 
        (float)this->getConstRenderWindow().getSize().y
        };
    auto buttonSpace = sf::FloatRect{
        this->position, this->size
    };
    
    return Geometry::spaceTransform(
        mousePosition, screenSpace, buttonSpace
        );
}