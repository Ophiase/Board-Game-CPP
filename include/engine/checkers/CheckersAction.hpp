#pragma once


#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/Board.hpp"
#include "CheckersState.hpp"


using CellPath = std::vector<CellPosition>;

class CheckersManager;

class CheckersAction : public Action<CheckersManager, Board> {
	private:
        CheckersState applyPawn(CheckersState) const;
        CheckersState applyKing(CheckersState) const;
    public:
        static const std::vector<CellPosition> authorizedOffsets;
        const CellPath jumps;

        // -----------------------------------------------

        CheckersAction(
            const CheckersManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<CheckersManager, Board>{manager, author, step}, 
        jumps{jumps} {};
        
        CheckersAction(const CheckersAction & other) :
        Action{other}, jumps{other.jumps} {};

        void removePointsFromScore(Board board, int & score) const;

        // ----------------------------------------------- 
        // OVERRIDES

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
};
