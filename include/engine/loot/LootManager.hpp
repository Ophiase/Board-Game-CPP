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

        /*
            Given a capture/visited, we want to add to
            current captures/visiteds a all the pair such that :
                - the pair wasn't previously found
                - the pair extend the given pair 
        */
        void expandCombination(
            std::vector<CapturePath> & result,
            std::vector<CapturePath> & currentCapturesPath,
            CapturePath capturePathToExpand,
            Board board
        ) const;

        /*
            For a given yellow pawn, we want to know
            all the combinations of positions captured
            we can form (up to isomorphism).
        */
        std::vector<CapturePath> combinationsOfCapture(
                CellPosition axiom, Board board) const;
        
        /*
            We want to know every paths (up to isomorphism) that can be build
            starting on input. Input paths should only have 1 element.
        */
        std::vector<CellPath> expendPaths(std::vector<CellPath>, Board board) const;

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
        std::tuple<Board, ScoreList> evaluateAction(
            LootAction action, Board board) const override;
};