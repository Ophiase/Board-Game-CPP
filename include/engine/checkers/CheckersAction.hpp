#pragma once


#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/Board.hpp"
#include "CheckersState.hpp"
#include "engine/Combination.hpp"

using CellPath = std::vector<CellPosition>;

class CheckersManager;

class CheckersAction : public Action<CheckersManager, Board> {
	private:
        static bool equivalentCellPath(
            const CellPath &, const CellPath & 
        );

        static void completeSpecificPawnActions(
            const CheckersManager * manager, 
            const CheckersState &,

            std::vector<CellPath> & visited,
            std::vector<CellPath> & nextVisited,
            CellPath currentPath
        );

        static void completeSpecificQueenActions(
            const CheckersManager * manager, 
            const CheckersState &,

            std::vector<CellPath> & visited,
            std::vector<CellPath> & nextVisited,
            CellPath currentPath
        );

        static std::vector<CheckersAction> getSpecificActions(
            const CheckersManager * manager, 
            const CheckersState&,
            CellPosition axiom);

        static std::vector<CheckersAction> getCaptures(
            const CheckersManager * manager, const CheckersState&);

        static std::vector<CheckersAction> getPawnMoves(
            const CheckersManager * manager, const CheckersState&);

        static std::vector<CheckersAction> getQueenMoves(
            const CheckersManager * manager, const CheckersState&);


        bool isValidQueenCapture(const CheckersState &) const;
        bool isValidQueenAction(const CheckersState &) const;
        bool isValidPawnAction(const CheckersState &) const;
        
        static CellPosition normalizeJumpOffset(CellPosition);
    public:
        static const std::vector<CellPosition> allPawnOffsets;
        static const std::vector<CellPosition> directWhitePawnOffsets;
        static const std::vector<CellPosition> directBlackPawnOffsets;
        static const std::vector<CellPosition> jumpPawnOffsets;
        static const std::vector<CellPosition> directQueenOffsets;
        
        const CellPath jumps;
        const bool surrend;

        // -----------------------------------------------

        CheckersAction(
            const CheckersManager *manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<CheckersManager, Board>{manager, author, step}, 
        jumps{jumps}, surrend{false} {};

        /* surrender constructor */
        CheckersAction(
            const CheckersManager *manager, PlayerId author, uint step) :
        Action<CheckersManager, Board>{manager, author, step}, surrend{true} {};

        CheckersAction(const CheckersAction & other) :
        Action<CheckersManager, Board>{other}, jumps{other.jumps}, 
        surrend{other.surrend} {};

        Combination toCaptured(CheckersState) const;

        // ----------------------------------------------- 
        // OVERRIDES

        /*
            All possibles action from current configuration (up to isomorphism).
        */
        static std::vector<CheckersAction> getActions(
            const CheckersManager * manager, CheckersState);

        /*
            Is there any authorized/correct action ?
        */
        static bool hasRemainingActions(
            const CheckersManager *, CheckersState);

        /*
            Is action authorized/correct.
        */
        bool  isValid(CheckersState) const override;
        
        /*
            If action is not valid, throw an error.
        */
        CheckersState apply(CheckersState) const override;
        std::string toString() const override;

        /*
            Doe 2 actions have the same effect on the board?
        */
        bool actionEquivalence(CheckersState, const CheckersAction &other) const;

};
