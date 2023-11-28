#pragma once

#include <string>
#include "Canvas.hpp"

class Text : virtual public Canvas {
    protected:
        std::string text;

        /*
            Calulcate width, according to height and text length.
        */
        float getWidth() const;
        
        /*
            Percent of overlap between two characters.
        */
        const float overlap{0.5};
    public:
        Text(Container *parent, 
            std::string text,
            sf::Vector2f position, float ySize,
            bool withHandler = false
            );

        void setText(std::string text);
        std::string getText() const;

        void setSize(sf::Vector2f v);
        
        void draw() override;
};