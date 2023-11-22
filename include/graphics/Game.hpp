#pragma once

#include "graphics/Screen.hpp"

class Game : Screen {
    protected:
        Game();
    public:
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual Screen *handleEvent(sf::Event event) = 0;
};