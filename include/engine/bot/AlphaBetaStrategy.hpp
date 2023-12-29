#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"
#include <limits>

/*
    Alpha Beta Pruning (optimization of minmax algorithm).
    
    Works only with two opponents on zero sum game.
    - https://stackoverflow.com/a/63609301
*/
template <class ActionType, class BoardType, class ManagerType>
class AlphaBetaStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        const uint maxDepth;

        AlphaBetaStrategy(const ManagerType* manager, uint maxDepth = 3);
        
        ~AlphaBetaStrategy() override = default;

        ActionType play(GameState<BoardType>) override;

        std::tuple<ActionType, int> alphaBetaAlgorithm(
            GameState<BoardType>,
            int alpha = std::numeric_limits<int>::min(), 
            int beta = std::numeric_limits<int>::max(), 
            uint depth = 0) const;
};

template <class ActionType, class BoardType, class ManagerType>
AlphaBetaStrategy<ActionType, BoardType, ManagerType>
::AlphaBetaStrategy(const ManagerType* manager, uint maxDepth) :
    Strategy<ActionType, BoardType, ManagerType>(manager), 
    maxDepth{maxDepth} 
{
    if (this->manager.players.size() != 2)
            throw NotImplemented();
};


template <class ActionType, class BoardType, class ManagerType>
ActionType AlphaBetaStrategy<ActionType, BoardType, ManagerType>::play(
    GameState<BoardType> state
) {

    throw NotImplemented();

    //return std::get<1>(alphaBetaAlgorithm(state));
};


template <class ActionType, class BoardType, class ManagerType>
std::tuple<ActionType, int> AlphaBetaStrategy<ActionType, BoardType, ManagerType>
::alphaBetaAlgorithm(GameState<BoardType> state, int alpha, int beta, uint depth) const
{
    bool maximize = depth & 0;

    std::vector<ActionType> actions = 
        ActionType::getActions(this->manager, state);

    throw NotImplemented();

    /*
    if (maximize) {
        int max = alpha;
        for (action : actions) {
            board.apply(action)
        }
    } else {
        int min = beta;
        for (int i = 0; i < actions.size(); i++) {

        }
    }
    */

};
