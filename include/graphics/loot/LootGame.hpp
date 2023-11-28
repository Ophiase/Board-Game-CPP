#pragma once

#include "graphics/Game.hpp"

class LootGame : public Game {
    protected:
    public:
        LootGame(Launcher *launcher) : Game{launcher, "Loot"} {};
};