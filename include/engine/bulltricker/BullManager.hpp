#pragma once

#include "engine/BoardSided.hpp"
#include "engine/Manager.hpp"
#include "BullAction.hpp"
#include "BullState.hpp"

/*
    Proportion of each type of pawn could be a paramater.
*/
class BullManager : public Manager<BullAction, Board> {
    private:
        Board initialBoard() override;
    public:
        using Manager::canPlayAction;

        const int TOTAL_WHITE_KING = 1;
        const int TOTAL_BLACK_KING = 1;
		const int TOTAL_WHITE_PAWN = 15;
		const int TOTAL_BLACK_PAWN = 15;
		const int TOTAL_WHITE_QUEEN = 8;
		const int TOTAL_BLACK_QUEEN = 8;

        const int WHITE_BONUS = 1;
        const int BLACK_BONUS = 1;

        static std::vector<Player> makePlayers(bool againstBot = false);
        BullManager(bool againstBot = false);

        bool canPlayAction(BullState) const override;
};