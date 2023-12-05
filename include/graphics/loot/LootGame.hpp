#pragma once

#include "engine/loot/LootAction.hpp"
#include "engine/loot/LootManager.hpp"
#include "graphics/Game.hpp"

class LootGame : public Game {
    protected:
        LootManager manager;
        std::vector<CellPosition> cacheAction;
        void playAction();
        void AIturn();

        /*
            Can player interact with the game ? 
        */
        bool interactive{false};

        void handleMouse(sf::Event);
        void handleKeyboard(sf::Event);
    public:
        LootGame(Launcher *launcher);
        void draw() override;
        void handleEvent(sf::Event) override;
};