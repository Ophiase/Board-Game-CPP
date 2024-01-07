#pragma once

#include "engine/BoardSided.hpp"
#include "engine/Manager.hpp"
#include "BullAction.hpp"
#include "BullState.hpp"

/*
    Proportion of each type of pawn could be a paramater.
*/
class BullManager : public Manager<BullAction, BoardSided> {
    private:
        BoardSided initialBoard() override;
    public:
        using Manager::canPlayAction;

        const int MAX_QUEEN_DEPTH = 3;

        const int TOTAL_WHITE_KING = 1;
        const int TOTAL_BLACK_KING = 1;
		const int TOTAL_WHITE_PAWN = 15;
		const int TOTAL_BLACK_PAWN = 15;
		const int TOTAL_WHITE_QUEEN = 8;
		const int TOTAL_BLACK_QUEEN = 8;

        const int WHITE_BONUS = 1;
        const int BLACK_BONUS = 1;

        const int H_WIDTH = 7;
        const int H_LENGTH = 8;
        const int V_WIDTH = 8;
        const int V_LENGTH = 7;

        static std::vector<Player> makePlayers(bool againstBot = false);
        BullManager(bool againstBot = false);

        bool canPlayAction(BullState) const override;
};