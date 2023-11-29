#pragma once

#include <string>
#include "Canvas.hpp"
#include <SFML/Graphics.hpp>

class Text : virtual public Canvas {
    public:
        const sf::Color DEFAULT_COLOR{255, 240, 210, 255};
    protected:
        std::string text;

        /*
            Calulcate width, according to height and text length.
        */
        float getWidth() const;
        
        /*
            Percent of overlap between two characters.
        */
        const float overlap{0.6f};
        sf::Color color{DEFAULT_COLOR};
    public:
        Text(Container *parent, 
            std::string text,
            sf::Vector2f position = sf::Vector2f{0.0, 0.0}, 
            float ySize = 0.1,
            bool withHandler = false
            );

        void setText(std::string text, bool center=false);
        std::string getText() const;

        void setSize(sf::Vector2f v);
        
        void draw() override;
};