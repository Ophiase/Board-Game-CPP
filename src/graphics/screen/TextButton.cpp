#include <SFML/Graphics.hpp>

#include "geometry/Geometry.hpp"
#include "graphics/screen/TextButton.hpp"
#include "utils/NotImplemented.hpp"

void TextButton::draw() {
    this->getRenderWindow().draw(this->text);
}

void TextButton::fitInside(sf::FloatRect rect) {
    throw NotImplemented();
    
    auto bbox = this->text.getLocalBounds();
    auto result = Geometry::fitInside(
        sf::Vector2f(bbox.width, bbox.height), rect
        );
    
    this->text.setPosition(result.left, result.top); 
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