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

        static std::vector<CellPosition> getSurroundingCells(const BullState &, CellPosition pos);

        static bool isThereAKingCapture(
            BullAction action, CellPosition kingPosition);

        static std::vector<BullAction> getPawnMoves(
            const BullManager * manager, const BullState&);

        static std::vector<BullAction> getQueenMoves(
            const BullManager * manager, const BullState&);

        static void getQueenStraightMove(
            const BullManager *manager, const BullState & state,
            SidePosition position, std::vector<BullAction> & actions,
            const SideVector dir
        );

        static void getPawnCaptures(
            const BullManager *manager, const BullState & state,
            SidePosition position, std::vector<BullAction> & actions
        );

        static void getQueenCaptures(
            const BullManager *manager, const BullState & state,
            SidePosition position, std::vector<BullAction> & actions,
            const SideVector dir
        );


        static std::vector<SidePosition> getPawnHorizontalSpecificMoves(
            SideVector position, bool isWhite, bool initPosition);

        static std::vector<SidePosition> getPawnVerticalSpecificMoves(
            SideVector position, bool isWhite);

        static void getPawnHorizontalMoves(
            const BullManager *manager, const BullState & state,
            SidePosition position, std::vector<BullAction> & actions
        );

        static void getPawnVerticalMoves(
            const BullManager *manager, const BullState & state,
            SidePosition position, std::vector<BullAction> & actions
        );

        static void getQueenHorizontalMoves(
            const BullManager *manager, const BullState & state,
            SidePosition position, std::vector<BullAction> & actions
        );

        static void getQueenVerticalMoves(
            const BullManager *manager, const BullState & state,
            SidePosition position, std::vector<BullAction> & actions
        );

        static void getKingMoves(
            const BullManager *manager, const BullState & state,
            CellPosition position, std::vector<BullAction> & actions
        );

        static std::vector<BullAction> getMoves(const BullManager *, const BullState &);

        static std::vector<BullAction> getCaptures(
            const BullManager *, const BullState&);

        static bool isSurrounded(const BullManager * manager, BullState state, CellPosition pos) ;
   
    public:
        static const std::vector<CellPosition> authorizedKingOffsets;
        static const std::vector<CellPosition> surroundingCellsOffsets;

        const CellPath cellJumps;
        const SidePath sideJumps;
        const bool isSidePath;
        const bool surrend;

        // -----------------------------------------------

        BullAction(
            const BullManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<BullManager, BoardSided>{manager, author, step}, 
        cellJumps{jumps}, isSidePath{0}, surrend{false} {};

        BullAction(
            const BullManager * manager, PlayerId author, uint step, 
            SidePath jumps) :
        Action<BullManager, BoardSided>{manager, author, step}, 
        sideJumps{jumps}, isSidePath{1}, surrend{false} {};

        BullAction(const BullManager * manager, PlayerId author, uint step) :
        Action<BullManager, BoardSided>{manager, author, step}, 
        isSidePath{false}, surrend{true} {};

        BullAction(const BullAction & other) :
        Action{other}, cellJumps{other.cellJumps}, sideJumps{other.sideJumps},
        isSidePath{other.isSidePath}, surrend{other.surrend} {};


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
