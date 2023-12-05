#pragma once

#include "engine/Board.hpp"
#include "engine/Manager.hpp"
#include "LootAction.hpp"

/*
    Proportion of each type of pawn could be a paramater.
*/
class LootManager : public Manager<LootAction, Board> {
    private:
        CellPiece randomCellPiece(int & remaningYellow, 
            int & remaningRed, int & remaningBlack);
        Board initialBoard() override;
    public:
        const int TOTAL_YELLOW_PAWN = 34;
        const int TOTAL_RED_PAWN = 20;
        const int TOTAL_BLACK_PAWN = 10;

        static std::vector<Player> makePlayers(int n);
        LootManager(int nPlayers = 2);

        std::vector<LootAction> getActions() const override;
        bool canPlayAction(LootAction action) const override;
        Board evaluateAction(
            LootAction action, Board board) const override;
        
};