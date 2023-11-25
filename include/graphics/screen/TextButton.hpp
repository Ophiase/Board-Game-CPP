#pragma once

#include "Button.hpp"

class TextButton : public Button {
    protected:
        sf::FloatRect targetBbox;
        sf::Text textObject;
        void update();
    public:
        TextButton() = delete;
        TextButton(
            Container *container, sf::Vector2f position, 
            sf::Vector2f size) = delete;
        
        TextButton(
            Container *container, 
            std::string text, 
            sf::FloatRect targetBbox,
            sf::Font font
            );

        void draw();
        void fit(sf::FloatRect targetBbox);

        std::string getTextContent(void);
        void setTextContent(std::string textContent);

        sf::Vector2f getRelativeMousePosition() const;
};