#pragma once

#include "graphics/screen/Screen.hpp"
#include "SFML/Graphics.hpp"

class Menu : public Screen {
    public:
        std::string getTitle() override;
        Screen *successor{nullptr};
};