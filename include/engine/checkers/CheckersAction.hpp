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
        static std::vector<CheckersAction> getPawnMoves(
            const CheckersManager * manager, CheckersState);

        static std::vector<CheckersAction> getQueenMoves(
            const CheckersManager * manager, CheckersState);

        static std::vector<CheckersAction> getPawnCaptures(
            const CheckersManager * manager, CheckersState);

        static std::vector<CheckersAction> getQueenCaptures(
            const CheckersManager * manager, CheckersState);

        bool isValidPawnMove(CheckersState) const;
        bool isValidQueenMove(CheckersState) const;
    public:
        static const std::vector<CellPosition> authorizedOffsets;
        static const std::vector<CellPosition> directOffsets;
        static const std::vector<CellPosition> jumpOffsets;
        
        const CellPath jumps;

        // -----------------------------------------------

        CheckersAction(
            const CheckersManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<CheckersManager, Board>{manager, author, step}, 
        jumps{jumps} {};
        
        CheckersAction(const CheckersAction & other) :
        Action<CheckersManager, Board>{other}, jumps{other.jumps} {};

        void removePointsFromScore(Board board, int & score) const;

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
};
