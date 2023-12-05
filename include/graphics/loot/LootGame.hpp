#pragma once

#include "engine/loot/LootAction.hpp"
#include "engine/loot/LootManager.hpp"
#include "graphics/Game.hpp"

class LootGame : public Game {
    protected:
        LootManager manager;
        std::vector<CellPosition> cacheAction;

        /*
            Reset interface on begining of a turn.
        */
        void startTurn();

        /*
            If cache is empty, it cancel the last action,
            otherwise it empty the cache. 
        */
        void cancelAction();

        /*
            Launch the action in cache.
        */
        void playAction();

        /*
            Delegate turn to an AI.
        */
        void AIturn();

        /*
            Can the user interact with the game ? 
        */
        bool interactive{false};

        void handleMouse(sf::Event);
        void handleKeyboard(sf::Event);
        void handleCheckerBoard();
    public:
        LootGame(Launcher *launcher);
        void draw() override;
        void handleEvent(sf::Event) override;
};