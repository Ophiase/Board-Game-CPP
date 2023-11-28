#pragma once

#include "Button.hpp"
#include "Text.hpp"

class TextButton : public Button, public Text {
    protected:
    public:
        TextButton(Container *parent,
            std::string text,
            std::function<void(sf::Event)> onClickAction = [](sf::Event) {},
            std::function<void(sf::Event)> onHoverAction = [](sf::Event) {}, 
            sf::Vector2f position = sf::Vector2f{0.0, 0.0}, 
            float ySize = 0.1,
            bool withHandler = true
        ) : 
        Canvas{parent, position, sf::Vector2f{1.0, ySize}, withHandler},
        Button{parent, onHoverAction, onClickAction},
        Text{parent, text} {};
};