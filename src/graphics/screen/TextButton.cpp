#include <SFML/Graphics.hpp>

#include "geometry/Geometry.hpp"
#include "graphics/screen/TextButton.hpp"

void TextButton::draw() {
    auto a = sf::RectangleShape{sf::Vector2f{1.0, 1.0}};
    a.setFillColor(sf::Color::Red);
    
    std::cout << "called" << std::endl;
    window.draw(a);
    window.draw(this->text);
}

sf::Vector2f TextButton::getRelativeMousePosition() const {
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