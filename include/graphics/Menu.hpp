#pragma once

#include "graphics/Screen.hpp"
#include "SFML/Graphics.hpp"

class Menu : public Screen {
    public:
        void draw(sf::RenderWindow& window);
        Screen *handleEvent(sf::Event event);
};