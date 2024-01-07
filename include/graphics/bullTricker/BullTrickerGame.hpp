#pragma once

#include "graphics/Game.hpp"
#include "engine/bullTricker/BullManager.hpp"
#include "engine/bullTricker/BullAction.hpp"
#include "engine/bot/Bot.hpp"

class BullTrickerGame : public Game {
    protected:
        BullManager manager;
        std::vector<Bot<BullAction, BoardSided, BullManager>*> bots;
        std::vector<CellPosition> cachedCellAction;
        std::vector<SidePosition> cachedSideAction;

        void applyAction(BullAction action);

        void startTurn() override;
        void cancelAction() override;
        void playAction() override;

        void AIinit() override;
        void AIturn() override;

        bool handleMouse(sf::Event);
        bool handleKeyboard(sf::Event);
        void handleCheckerBoard();
        
    public:
        BullTrickerGame(Launcher *launcher, bool againstBot=false);
        ~BullTrickerGame();

        void draw() override;
        bool handleEvent(sf::Event) override;
        
        void updateBoardSidedContent(BoardSided);
};