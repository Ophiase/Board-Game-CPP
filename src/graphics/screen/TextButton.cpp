#include <SFML/Graphics.hpp>

#include "geometry/Geometry.hpp"
#include "graphics/screen/TextButton.hpp"
#include "utils/NotImplemented.hpp"
#include "utils/Cli.hpp"

#define FONT_RESOLUTION 100U
#define FONT_SCALE_CORRECTION 0.0122f
#define FONT_Y_CORRECTION 0.25f

TextButton::TextButton(
    Container *container, 
    std::string text, 
    sf::FloatRect targetBbox,
    sf::Font *font
    ) : Button{container}, targetBbox{targetBbox} {

    this->textObject = sf::Text{text, *font, FONT_RESOLUTION};
    update();

    container->add(this);
}

void TextButton::update() { // TODO (hard)
    /*
        returns a result relative to 100U
    */
    auto bbox = this->textObject.getGlobalBounds();
    auto result = Geometry::fitInside(
        sf::Vector2f(bbox.width, bbox.height), targetBbox
        );
    
    this->targetBbox = result;
    this->textObject.setPosition(result.left, result.top - result.height*FONT_Y_CORRECTION); 
    this->textObject.setScale(sf::Vector2f{
        result.height * FONT_SCALE_CORRECTION, result.height * FONT_SCALE_CORRECTION});

}

std::string TextButton::getTextContent(void) {
    return this->textObject.getString();
}

void TextButton::setTextContent(std::string textContent) {
    this->textObject.setString(textContent);
    this->update();
}


void TextButton::draw() {
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