#pragma once

#include "graphics/Game.hpp"

class BullTrickerGame : public Game {
    public:
        BullTrickerGame(Launcher *launcher) : Game{launcher, "BullTricker"} {};
};