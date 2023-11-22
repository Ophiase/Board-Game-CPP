#pragma once

#include "graphics/screen/Screen.hpp"
#include "SFML/Graphics.hpp"

class Menu : public Screen {
    public:
        Menu(sf::RenderWindow & window);

        std::string getTitle() override;
        Screen *successor{nullptr};
};