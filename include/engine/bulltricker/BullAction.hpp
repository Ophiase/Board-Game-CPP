#pragma once


#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/BoardSided.hpp"
#include "BullState.hpp"
#include "engine/type/CellPath.hpp"
#include "engine/type/SidePath.hpp"

class BullManager;

class BullAction : public Action<BullManager, BoardSided> {
	private:
        bool isValidQueenAction(const BullState &) const;
        bool isValidPawnAction(const BullState &) const;
        bool isValidWhitePawnAction(const BullState &) const;
        bool isValidBlackPawnAction(const BullState &) const;
        bool isValidKingAction(const BullState &) const;
        std::vector<CellPosition> getSurroundingCells(const BullState &, CellPosition pos) const;

        static std::vector<BullAction> getSpecificActions(
            const BullManager * manager, 
            const BullState&,
            CellPosition axiom);

        static std::vector<BullAction> getSpecificActions(
            const BullManager * manager, 
            const BullState&,
            SidePosition axiom);

    public:
        static const std::vector<CellPosition> authorizedKingOffsets;
        static const std::vector<CellPosition>surroundingCellsOffsets;

        const CellPath cellJumps;
        const SidePath sideJumps;
        const bool isSidePath;


        // -----------------------------------------------

        BullAction(
            const BullManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<BullManager, BoardSided>{manager, author, step}, 
        cellJumps{jumps}, isSidePath{0} {};

        BullAction(
            const BullManager * manager, PlayerId author, uint step, 
            SidePath jumps) :
        Action<BullManager, BoardSided>{manager, author, step}, 
        sideJumps{jumps}, isSidePath{1} {};

        BullAction(const BullAction & other) :
        Action{other}, cellJumps{other.cellJumps}, sideJumps{other.sideJumps},
        isSidePath{other.isSidePath} {};

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
