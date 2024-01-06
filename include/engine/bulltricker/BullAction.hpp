#pragma once


#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/BoardSided.hpp"
#include "BullState.hpp"
#include "engine/type/CellPath.hpp"


using SidePath = std::vector<SidePosition>;

class BullManager;

class BullAction : public Action<BullManager, BoardSided> {
	private:
        bool isValidQueenAction(const BullState &) const;
        bool isValidPawnAction(const BullState &) const;
        bool isValidWhitePawnAction(const BullState &) const;
        bool isValidBlackPawnAction(const BullState &) const;
        bool isValidKingAction(const BullState &) const;
        std::vector<CellPosition> getSurroundingCells(const BullState &, CellPosition pos) const;

    public:
        static const std::vector<CellPosition> authorizedKingOffsets;
        static const std::vector<CellPosition>surroundingCellsOffsets;

        const CellPath jumps;


        // -----------------------------------------------

        BullAction(
            const BullManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<BullManager, BoardSided>{manager, author, step}, 
        jumps{jumps} {};
        
        BullAction(const BullAction & other) :
        Action{other}, jumps{other.jumps} {};

        // ----------------------------------------------- 
        // OVERRIDES

        /*
            All possibles action from current configuration (up to isomorphism).
        */
        static std::vector<BullAction> getActions(
            const BullManager * manager, BullState);

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
