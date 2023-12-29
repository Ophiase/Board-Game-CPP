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