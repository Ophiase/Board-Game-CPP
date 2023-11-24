#pragma once

#include "graphics/screen/Screen.hpp"
#include "SFML/Graphics.hpp"

class Menu : public Screen {
    public:
        Menu(Launcher *launcher);

        std::string getTitle() override;
        Screen *successor{nullptr};
};