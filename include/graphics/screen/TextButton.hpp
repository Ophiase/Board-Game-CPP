#pragma once

#include "Button.hpp"

class TextButton : public Button {
    protected:
        sf::Text text;
    public:
        TextButton() = delete;
        TextButton(
            Container *container, sf::Vector2f position, 
            sf::Vector2f size) = delete;
        TextButton(Container *container, sf::Text text, bool filled=true) : 
            Button{container}, text{text} { (void)filled; };

        void draw();

        sf::Vector2f getRelativeMousePosition() const;
};