#pragma once

#include "graphics/screen/Screen.hpp"
#include "SFML/Graphics.hpp"

class Menu : public Screen {
    public:
        std::string getTitle();
        
        void draw(sf::RenderWindow& window);
        void handleEvent(sf::Event event);

        Screen *successor{nullptr};
};