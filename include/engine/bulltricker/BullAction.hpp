#pragma once


#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/BoardSided.hpp"
#include "BullState.hpp"


using CellPath = std::vector<CellPosition>;

class BullManager;

class BullAction : public Action<BullManager, BoardSided> {
	private:
        
    public:
        static const std::vector<CellPosition> authorizedOffsets;
        const CellPath jumps;

        // -----------------------------------------------

        BullAction(
            const BullManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<BullManager, BoardSided>{manager, author, step}, 
        jumps{jumps} {};
        
        BullAction(const BullAction & other) :
        Action{other}, jumps{other.jumps} {};

        void removePointsFromScore(Board board, int & score) const;

        // ----------------------------------------------- 
        // OVERRIDES

        /*
            Is there any authorized/correct action ?
        */
        static bool hasRemainingActions(
            const BullManager *, BullState);

        /*
            Is action authorized/correct.
        */
        bool  isValid(BullState) const override;
        
        /*
            If action is not valid, throw an error.
        */
        BullState apply(BullState) const override;
        std::string toString() const override;
};