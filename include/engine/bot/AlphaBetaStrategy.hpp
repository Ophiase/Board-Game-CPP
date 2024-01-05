#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"
#include <limits>
#include <algorithm>
#include <tuple>
#include <memory>

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

        std::tuple<ActionType*, int> alphaBetaAlgorithm(
            GameState<BoardType>, PlayerId scoreId,
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
        throw std::invalid_argument("AlphaBeta require 1v1 zero sum game.");
    
    if (maxDepth < 1)
        throw std::invalid_argument("Invalid maxDepth");
};


template <class ActionType, class BoardType, class ManagerType>
ActionType AlphaBetaStrategy<ActionType, BoardType, ManagerType>::play(
    GameState<BoardType> state
) {
    return *std::unique_ptr<ActionType>(
        std::get<0>(alphaBetaAlgorithm(state), state.player)
    );
};

/*
    There is no std::optional in c++11, 
    so this function cannot returns <nullopt, score>.
    Instead we use a pointer of ActionType.
*/
template <class ActionType, class BoardType, class ManagerType>
std::tuple<ActionType*, int> AlphaBetaStrategy<ActionType, BoardType, ManagerType>
::alphaBetaAlgorithm(
    GameState<BoardType> state,  PlayerId scoreId,
    int alpha, int beta, uint depth
) const {
    if ((this->manager->isFinished(state)) || (depth == maxDepth))
        return state.scores[scoreId];

    bool maximize = depth & 0;

    std::vector<ActionType> actions = 
        ActionType::getActions(this->manager, state);
    if (actions.size() == 0) // safe check
        throw std::invalid_argument("No action availibles");
    
    int actionIndex = 0;
    int value = 0;

    if (maximize) {
        value = std::numeric_limits<int>::min();
        for (int i = 0; i < actions.size(); i++) {
            auto currentAction = actions[i];
            GameState<BoardType> nextState = currentAction.apply(state);
            int nextScore = alphaBetaAlgorithm(nextState, scoreId, alpha, beta, depth+1);
            if (value > beta) break;
            if (value > nextScore) continue;
            
            actionIndex = i;
            value = nextScore;
        }
    } else {
        value = std::numeric_limits<int>::max();
        for (int i = 0; i < actions.size(); i++) {
            auto currentAction = actions[i];
            GameState<BoardType> nextState = currentAction.apply(state);
            int nextScore = alphaBetaAlgorithm(nextState, scoreId, alpha, beta, depth+1);
            if (value < alpha) break;
            if (value < nextScore) continue;
            
            actionIndex = i;
            value = nextScore;
        }
    }

    ActionType *action = (depth == 0) ?
        new ActionType{actions[actionIndex]} :
        nullptr;

    return std::make_tuple(action, value);
};
