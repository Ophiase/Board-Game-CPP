#pragma once

#include "graphics/screen/Screen.hpp"

class Game : public Screen {
    public:
        Game(Launcher *launcher, std::string title="Game") : Screen{launcher, title} {};
};