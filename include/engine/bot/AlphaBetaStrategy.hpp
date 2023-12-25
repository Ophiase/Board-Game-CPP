#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"

template <class ActionType, class BoardType, class ManagerType>
class AlphaBetaStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        AlphaBetaStrategy() : 
            Strategy<ActionType, BoardType, ManagerType>() {};
        
        ~AlphaBetaStrategy() override = default;

        ActionType play(
            const ManagerType*, 
            PlayerId, 
            uint step, 
            BoardType) override;
};

template <class ActionType, class BoardType, class ManagerType>
ActionType AlphaBetaStrategy<ActionType, BoardType, ManagerType>::play(
    const ManagerType *manager,
    PlayerId id,
    uint step,
    BoardType board
) {
    std::vector<ActionType> actions = 
        ActionType::getActions(manager, id, step, board);

    throw NotImplemented();
};