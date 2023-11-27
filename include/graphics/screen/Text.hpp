#pragma once

#include <string>
#include "Canvas.hpp"

class Text : virtual public Canvas {
    protected:
        std::string text;
    public:
        Text(Container *parent, 
            std::string text,
            sf::Vector2f position, float ySize,
            bool withHandler = false
            );

        void setText(std::string text);
        std::string getText() const;

        void adjustXSize();
        void draw();
};