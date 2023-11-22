#pragma once

#include "Button.hpp"

class TextButton : public Button {
    protected:
        sf::Text text;
    public:
        TextButton() = delete;
        TextButton(
            sf::RenderWindow & window, sf::Vector2f position, 
            sf::Vector2f size) = delete;
        TextButton(sf::RenderWindow & window, sf::Text text, bool filled=true) : 
            Button{window}, text{text} { (void)filled; };

        void draw();

        sf::Vector2f getRelativeMousePosition() const;
};