#pragma once

#include "engine/loot/LootAction.hpp"
#include "engine/loot/LootManager.hpp"
#include "graphics/Game.hpp"

class LootGame : public Game {
    protected:
        LootManager manager;
        std::vector<CellPosition> cacheAction;

        void startTurn() override;
        void cancelAction() override;
        void playAction() override;
        void AIturn() override;

        bool handleMouse(sf::Event);
        bool handleKeyboard(sf::Event);
        void handleCheckerBoard();

    public:
        LootGame(Launcher *launcher, int nPlayers = 2, int nBots = 0);
        void draw() override;
        bool handleEvent(sf::Event) override;
        
        void updateBoardContent(Board);
};