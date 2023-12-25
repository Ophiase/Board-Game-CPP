#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"

template <class ActionType, class BoardType, class ManagerType>
class MonteCarloStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        MonteCarloStrategy() : 
            Strategy<ActionType, BoardType, ManagerType>() {};
        
        ~MonteCarloStrategy() override = default;

        ActionType play(
            const ManagerType*, 
            PlayerId, 
            uint step, 
            BoardType) override;
};

template <class ActionType, class BoardType, class ManagerType>
ActionType MonteCarloStrategy<ActionType, BoardType, ManagerType>::play(
    const ManagerType *manager,
    PlayerId id,
    uint step,
    BoardType board
) {
    srand(time(NULL));
    std::vector<ActionType> actions = 
        ActionType::getActions(manager, id, step, board);

    throw NotImplemented();
};