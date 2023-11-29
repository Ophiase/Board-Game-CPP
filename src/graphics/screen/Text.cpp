#include "graphics/screen/Text.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "utils/Cli.hpp"

Text::Text(Container *parent, 
    std::string text,
    sf::Vector2f position, float ySize,
    bool withHandler
) : Canvas {
    parent, position, sf::Vector2f{1.0, ySize}, withHandler
} {
    this->setText(text);

};

std::string Text::getText() const {
    return this->text;
};

float Text::getWidth() const {
    return this->size.y * 
        (overlap + (this->text.size()*(1.0-overlap)));
}

void Text::setText(std::string text, bool center) {
    if (center) {
        auto m = this->getMid();
        this->setText(text);
        this->center(m);
        return;
    }

    this->text = text;
    this->size.x = this->getWidth();
};

/*
    It will only rescale sy not sx.
*/
void Text::setSize(sf::Vector2f v) {
    setSizeY(v.y);
};

void Text::draw() {
    float width = this->getSize().y * (1.0-overlap);
    auto defaultChar = ResourcesLoader::getCharTexture(' ');
    
    sf::Sprite sprite;
    sprite.setTexture(*defaultChar);
    sprite.setPosition(0.0f, 0.0f);
    sprite.setScale(sf::Vector2f(
        this->getSize().y / defaultChar->getSize().x, 
        this->getSize().y / defaultChar->getSize().y
        ));
        
    sprite.setColor(color);

    /*
        sf::RectangleShape rect;
        rect.setFillColor(sf::Color::Red);
        rect.setPosition(this->getPosition());
        rect.setSize(this->getSize());
        this->getRenderWindow().draw(rect);
    */
    
    for (uint i = 0; i < this->text.size(); i++) {
        char c = this->text.at(i);
        sprite.setTexture(
            *ResourcesLoader::getCharTexture(c));

        sprite.setPosition(
            this->getPosition().x + ((float)i * width),
            this->getPosition().y
        );   

        this->getRenderWindow().draw(sprite);
    }
};