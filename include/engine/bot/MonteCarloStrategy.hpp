#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"

template <class ActionType, class BoardType, class ManagerType>
class MonteCarloStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        MonteCarloStrategy(const ManagerType* manager) : 
            Strategy<ActionType, BoardType, ManagerType>(manager) {};
        
        ~MonteCarloStrategy() override = default;

        ActionType play(GameState<BoardType>) override;

        // needs a tree representation of exploration
        // needs a method to choose which node to explore
        // needs a method to random walk a node, state -> final score
            // needs a function final score -> value
        // needs a method to update tree
};

template <class ActionType, class BoardType, class ManagerType>
ActionType MonteCarloStrategy<ActionType, BoardType, ManagerType>::play(
    GameState<BoardType> state
) {
    srand(time(NULL));
    std::vector<ActionType> actions = 
        ActionType::getActions(this->manager, state);

    throw NotImplemented();
};