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

        void removePointsFromScore(Board board, int & score) const;
    public:
        const int TOTAL_YELLOW_PAWN = 34;
        const int TOTAL_RED_PAWN = 20;
        const int TOTAL_BLACK_PAWN = 10;

        const int YELLOW_BONUS = 1;
        const int RED_BONUS = 2;
        const int BLACK_BONUS = 3;

        static std::vector<Player> makePlayers(int n);
        LootManager(int nPlayers = 2);

        std::vector<LootAction> getActions(Board) const override;
        bool canPlayAction(Board) const override;
        bool canPlayAction(LootAction action) const override;
        std::tuple<Board, scoreList> evaluateAction(
            LootAction action, Board board) const override;
};