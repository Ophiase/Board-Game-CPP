#pragma once

#include "graphics/Screen.hpp"
#include "SFML/Graphics.hpp"

class Menu : public Screen {
    public:
        std::string getTitle();
        void draw(sf::RenderWindow& window);
        Screen *handleEvent(sf::Event event);
};