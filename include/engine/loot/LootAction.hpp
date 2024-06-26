#pragma once


#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/Board.hpp"
#include "CapturePath.hpp"
#include "engine/Combination.hpp"
#include "LootState.hpp"

using CellPath = std::vector<CellPosition>;

class LootManager;

class LootAction : public Action<LootManager, Board> {
    private:
        /*
            Given a capture/visited, we want to add to
            current captures/visiteds a all the pair such that :
                - the pair wasn't previously found
                - the pair extend the given pair 
        */
        static void expandCombination(
            CellPosition axiom,
            std::vector<CapturePath> & result,
            std::vector<CapturePath> & currentCapturesPath,
            CapturePath capturePathToExpand,
            Board board
        );

        /*
            For a given yellow pawn, we want to know
            all the combinations of positions captured
            we can form (up to isomorphism).
        */
        static std::vector<CapturePath> combinationsOfCapture(
            CellPosition axiom, Board board);
        
        /*
            We want to know every paths (up to isomorphism) that can be build
            starting on input. Input paths should only have 1 element.
        */
        static std::vector<CellPath> expendPaths(std::vector<CellPath>, Board board);

    public:
        static const std::vector<CellPosition> authorizedOffsets;
        const CellPath jumps;

        // -----------------------------------------------

        LootAction(
            const LootManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<LootManager, Board>{manager, author, step}, 
        jumps{jumps} {};

        LootAction(
            const LootManager * manager, LootState state, 
            CellPath jumps) :
        Action<LootManager, Board>{manager, state.player, state.step}, 
        jumps{jumps} {};

        LootAction(const LootAction & other) :
        Action{other},  jumps{other.jumps} {};

        void removePointsFromScore(Board board, int & score) const;

        // ----------------------------------------------- 
        // OVERRIDES

        /*
            All possibles action from current configuration (up to isomorphism).
        */
        static std::vector<LootAction> getActions(
            const LootManager * manager, LootState);

        /*
            Is there any authorized/correct action ?
        */
        static bool hasRemainingActions(const LootManager *, LootState);

        /*
            Is action authorized/correct.
        */
        bool  isValid(LootState) const override;
        
        /*
            If action is not valid, throw an error.
        */
        LootState apply(LootState) const override;

        std::string toString() const override;
};