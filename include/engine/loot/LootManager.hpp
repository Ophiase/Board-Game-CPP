#pragma once

#include "engine/Board.hpp"
#include "engine/Manager.hpp"
#include "LootAction.hpp"

/*
    A Combination of CellPositions.
    Two combinations should be considered equals if they share
    the same elements (even wihout same order).
*/
using Combination = std::vector<CellPosition>;

/*
    Proportion of each type of pawn could be a paramater.
*/
class LootManager : public Manager<LootAction, Board> {
    private:

        CellPiece randomCellPiece(int & remaningYellow, 
            int & remaningRed, int & remaningBlack);
        Board initialBoard() override;

        /*
            For a given yellow pawn, we want to know
            all the combinations of positions captured
            we can form (up to isomorphism).
        */
        std::vector<Combination> combinationsOfCapture(CellPosition axiom) const;
        
        /*
            We want to know every paths (up to isomorphism) that can be build
            starting on input. Input paths should only have 1 element.
        */
        std::vector<CellPath> expendPaths(std::vector<CellPath>) const;

        void removePointsFromScore(Board board, int & score) const;
    public:
        const int TOTAL_YELLOW_PAWN = 34;
        const int TOTAL_RED_PAWN = 20;
        const int TOTAL_BLACK_PAWN = 10;

        const int YELLOW_BONUS = 1;
        const int RED_BONUS = 2;
        const int BLACK_BONUS = 3;

        static std::vector<Player> makePlayers(
            int nPlayers = 2, int nBots = 0);
        LootManager(int nPlayers = 2, int nBots = 0);

        std::vector<LootAction> getActions(Board) const override;
        bool canPlayAction(Board) const override;
        bool canPlayAction(LootAction action) const override;
        std::tuple<Board, scoreList> evaluateAction(
            LootAction action, Board board) const override;
};