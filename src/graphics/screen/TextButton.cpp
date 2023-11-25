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
    update();

    container->add(this);
}

void TextButton::update() {
    return;
    auto bbox = this->textObject.getLocalBounds();
    auto result = Geometry::fitInside(
        sf::Vector2f(bbox.width, bbox.height), targetBbox
        );
    
    this->textObject.setPosition(result.left, result.top); 
    this->textObject.setScale(sf::Vector2f{result.width, result.height});
}

std::string TextButton::getTextContent(void) {
    return this->textObject.getString();
}

void TextButton::setTextContent(std::string textContent) {
    this->textObject.setString(textContent);
    this->update();
}


void TextButton::draw() {
    textObject.setPosition(0.0, 0.0);
    //this->textObject = sf::Text{"hi", ResourcesLoader::getFont(Font::OpenSansBold), 30U};

    sf::RenderWindow *a = &this->getRenderWindow();
    a->draw(this->textObject);
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