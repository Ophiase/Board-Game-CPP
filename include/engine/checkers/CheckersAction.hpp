#pragma once


#include "engine/Action.hpp"
#include "engine/Cell.hpp"
#include "engine/type/CellPosition.hpp"
#include "engine/Board.hpp"

using CellPath = std::vector<CellPosition>;

class CheckersManager;

class CheckersAction : public Action<CheckersManager, Board> {
	private:
        
    public:
        static const std::vector<CellPosition> authorizedOffsets;
        const CellPath jumps;

        // -----------------------------------------------

        CheckersAction(
            const CheckersManager * manager, PlayerId author, uint step, 
            CellPath jumps) :
        Action<CheckersManager, Board>{manager, author, step}, 
        jumps{jumps} {};

        void removePointsFromScore(Board board, int & score) const;

        // ----------------------------------------------- 
        // OVERRIDES

        /*
            Is there any authorized/correct action ?
        */
        static bool hasRemainingActions(
            const CheckersManager * manager, 
            PlayerId author, 
            uint step,
            Board board);

        /*
            Is action authorized/correct.
        */
        bool  isValid(Board) const override;
        
        /*
            If action is not valid, throw an error.
        */
        std::tuple<Board, ScoreList> apply(Board, ScoreList) const override;

        std::string toString() const override;
};
