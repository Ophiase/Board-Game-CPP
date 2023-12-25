#pragma once

#include "engine/loot/LootAction.hpp"
#include "engine/loot/LootManager.hpp"
#include "graphics/Game.hpp"
#include "engine/bot/Bot.hpp"

class LootGame : public Game {
    protected:
        LootManager manager;
        std::vector<Bot<LootAction, Board, LootManager>*> bots;
        std::vector<CellPosition> cacheAction;
        
        /*
            unsafe, apply an Action to the game.
        */
        void applyAction(LootAction action);

        void startTurn() override;
        void cancelAction() override;
        void playAction() override;

        void AIinit() override;
        void AIturn() override;

        bool handleMouse(sf::Event);
        bool handleKeyboard(sf::Event);
        void handleCheckerBoard();

    public:
        LootGame(Launcher *launcher, int nPlayers = 2, int nBots = 0);
        ~LootGame();

        void draw() override;
        bool handleEvent(sf::Event) override;
        
        void updateBoardContent(Board);
};