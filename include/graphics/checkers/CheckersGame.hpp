#pragma once

#include "graphics/Game.hpp"

class CheckersGame : public Game {
    public:
        CheckersGame(Launcher *launcher) : 
        Game{launcher, "Checkers"} {};
};