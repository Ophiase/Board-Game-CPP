#pragma once

#include "graphics/Game.hpp"
#include "engine/checkers/CheckersAction.hpp"
#include "engine/checkers/CheckersManager.hpp"
#include "graphics/Game.hpp"
#include "engine/bot/Bot.hpp"

class CheckersGame : public Game {
    protected:
        CheckersManager manager;
        std::vector<Bot<CheckersAction, Board, CheckersManager>*> bots;
        std::vector<CellPosition> cacheAction;

        void applyAction(CheckersAction action);

        void startTurn() override;
        void cancelAction() override;
        void playAction() override;

        void AIinit() override;
        void AIturn() override;

        bool handleMouse(sf::Event);
        bool handleKeyboard(sf::Event);
        void handleCheckerBoard();
        
    public:
        CheckersGame(Launcher *launcher, bool againstBot=false);
        ~CheckersGame();

        void draw() override;
        bool handleEvent(sf::Event) override;
        
        void updateBoardContent(Board);
};