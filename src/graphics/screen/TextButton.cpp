#include <SFML/Graphics.hpp>

#include "geometry/Geometry.hpp"
#include "graphics/screen/TextButton.hpp"
#include "utils/NotImplemented.hpp"
#include "utils/Cli.hpp"

void TextButton::handleEvent(sf::Event event) {
    auto mousePosition = this->getRelativeMousePosition();

    if (!Geometry::insideUnitBox(mousePosition)) {
        this->color = DEFAULT_COLOR;
        return;
    }

    this->color = DEFAULT_HOVER_COLOR;    
    this->onHoverFunction(event);
    if (event.type == sf::Event::MouseButtonPressed) 
        this->onClickFunction(event);
}