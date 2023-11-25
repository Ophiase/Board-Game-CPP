#pragma once

#include "graphics/screen/Screen.hpp"
#include "SFML/Graphics.hpp"
#include "utils/GarbageManager.hpp"

class Menu : public Screen {
    public:
        Menu(Launcher *launcher);

        std::string getTitle() override;
        Screen *successor{nullptr};

        void draw();
};