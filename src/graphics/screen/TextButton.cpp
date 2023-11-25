#include <SFML/Graphics.hpp>

#include "geometry/Geometry.hpp"
#include "graphics/screen/TextButton.hpp"
#include "utils/NotImplemented.hpp"
#include "utils/Cli.hpp"

TextButton::TextButton(
    Container *container, 
    std::string text, 
    sf::FloatRect targetBbox,
    sf::Font *font
    ) : Button{container}, targetBbox{targetBbox} {

    this->textObject = sf::Text{text, *font, 100U};
    this->textObject.setScale(0.01, 0.01);

    update();

    container->add(this);
}

void TextButton::update() { // TODO (hard)
    /*
        returns a result relative to 100U
    */
    auto bbox = this->textObject.getLocalBounds();
    auto result = Geometry::fitInside(
        sf::Vector2f(bbox.width, bbox.height), targetBbox
        );

    std::cout << result.top << " " << result.left << std::endl;
    std::cout << targetBbox.width << " " << targetBbox.height << std::endl;
    std::cout << bbox.width << " " << bbox.height << std::endl;
    std::cout << result.width << " " << result.height << std::endl;
    
    
    this->textObject.setPosition(result.left, result.top); 
    this->textObject.setScale(sf::Vector2f{
        result.height/100.0, result.height/100.0});
}

std::string TextButton::getTextContent(void) {
    return this->textObject.getString();
}

void TextButton::setTextContent(std::string textContent) {
    this->textObject.setString(textContent);
    this->update();
}


void TextButton::draw() {
    auto bbox = this->textObject.getLocalBounds();
    auto result = Geometry::fitInside(
        sf::Vector2f(bbox.width, bbox.height), targetBbox
        );

    sf::RectangleShape rect;
    Geometry::applyFloatRectToRectangleShape(rect, result);
    rect.setFillColor(sf::Color::Red);

    this->getRenderWindow().draw(rect);
    
    this->getRenderWindow().draw(this->textObject);
}

void TextButton::fit(sf::FloatRect targetBbox) {
    this->targetBbox = targetBbox;
    update();
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