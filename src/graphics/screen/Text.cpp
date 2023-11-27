#include "graphics/screen/Text.hpp"

Text::Text(Container *parent, 
    std::string text,
    sf::Vector2f position, float ySize,
    bool withHandler
) : Canvas{
    parent, position, sf::Vector2f{ySize, 1.0}, withHandler
}, text{text} {
    adjustXSize();
};

std::string Text::getText() const {
    return this->text;
};

void Text::setText(std::string text) {
    this->text = text;
    adjustXSize();
};

void Text::adjustXSize() {
    // TODO
};

void Text::draw() {
    // TODO

}