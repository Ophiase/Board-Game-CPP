#pragma once

#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/Board.hpp"

using CellPath = std::vector<CellPosition>;
class LootManager;

class LootAction : public Action<LootManager, Board> {
    private:
    public:
        static const std::vector<CellPosition> authorizedOffsets;
        const CellPath jumps;

        // -----------------------------------------------

        LootAction(
            const LootManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<LootManager, Board>{manager, author, step}, 
        jumps{jumps} {};

        // ----------------------------------------------- 

        /*
            All possibles action from current configuration (up to isomorphism).
        */
        static std::vector<LootAction> getActions(
            const LootManager * manager, 
            PlayerId author, 
            int step,
            Board board);

        /*
            Is there any authorized/correct action ?
        */
        static bool hasRemainingActions(
            const LootManager * manager, 
            PlayerId author, 
            int step,
            Board board);

        /*
            Is action authorized/correct.
        */
        bool  isValid(Board) const override;
        
        /*
            If action is not valid, throw an error.
        */
        Board apply(Board) const override;
};