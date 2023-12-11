#include "graphics/screen/Button.hpp"
#include "geometry/Geometry.hpp"
#include "utils/Cli.hpp"
#include <thread>

bool Button::handleEvent(sf::Event event) {
    auto mousePosition = this->getRelativeMousePosition();

    if (!Geometry::insideUnitBox(mousePosition)) 
        return false;
    
    this->onHoverFunction(event);
    if (event.type == sf::Event::MouseButtonReleased) {
        this->onClickFunction(event);
        return true;
    }

    return false;
}