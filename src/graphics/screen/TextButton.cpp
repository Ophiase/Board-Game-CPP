#include <SFML/Graphics.hpp>

#include "geometry/Geometry.hpp"
#include "graphics/screen/TextButton.hpp"

void TextButton::draw() {
    this->getRenderWindow().draw(this->text);
}

sf::Vector2f TextButton::getRelativeMousePosition() const {
    const sf::RenderWindow *window = &this->getConstRenderWindow();;

    auto mousePosition = Geometry::toFloat(sf::Mouse::getPosition());
    auto screenSpace = sf::FloatRect{
        0.0, 0.0, 
        (float)window->getSize().x, 
        (float)window->getSize().y
        };
    

    auto buttonSpace = sf::FloatRect{
        this->position, this->size
    };
    
    return Geometry::spaceTransform(
        mousePosition, screenSpace, buttonSpace
    );

}