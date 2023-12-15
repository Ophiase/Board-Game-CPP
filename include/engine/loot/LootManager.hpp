#pragma once

#include "engine/Board.hpp"
#include "engine/Manager.hpp"
#include "Combination.hpp"
#include "CapturePath.hpp"
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
        using Manager::canPlayAction;

        const int TOTAL_YELLOW_PAWN = 34;
        const int TOTAL_RED_PAWN = 20;
        const int TOTAL_BLACK_PAWN = 10;

        const int YELLOW_BONUS = 1;
        const int RED_BONUS = 2;
        const int BLACK_BONUS = 3;

        static std::vector<Player> makePlayers(
            int nPlayers = 2, int nBots = 0);
        LootManager(int nPlayers = 2, int nBots = 0);

        bool canPlayAction(PlayerId, uint step, Board) const override;
};