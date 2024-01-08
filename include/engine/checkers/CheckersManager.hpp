#pragma once

#include "engine/Board.hpp"
#include "engine/Manager.hpp"
#include "CheckersAction.hpp"
#include "CheckersState.hpp"

/*
    Proportion of each type of pawn could be a paramater.
*/
class CheckersManager : public Manager<CheckersAction, Board> {
    private:
        Board initialBoard() override;
    public:
        using Manager::canPlayAction;

        /* 
            It's necessary to impose limits when computing
            all the actions that a queen can performs
        */
        const int MAX_QUEEN_DEPTH = 3;
        const int MAX_QUEEN_BRANCHING = 2; // TODO

        const int TOTAL_WHITE_PAWN = 20;
        const int TOTAL_BLACK_PAWN = 20;

        const int WHITE_BONUS = 1;
        const int BLACK_BONUS = 1;

        static std::vector<Player> makePlayers(bool againstBot = false);
        CheckersManager(bool againstBot = false);

        bool canPlayAction(CheckersState) const override;
        std::vector<Player> getWinners() const;
};