#pragma once

#include "engine/loot/LootAction.hpp"
#include "engine/loot/LootManager.hpp"
#include "graphics/Game.hpp"

class LootGame : public Game {
    protected:
        LootManager manager;
        std::vector<CellPosition> cacheAction;
    public:
        LootGame(Launcher *launcher);
        void draw() override;
        void handleEvent(sf::Event) override;
        void playAction();
};