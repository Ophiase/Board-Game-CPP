#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"

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

        AlphaBetaStrategy(uint maxDepth = 3) : 
            Strategy<ActionType, BoardType, ManagerType>(), 
            depth{depth} {};
        
        ~AlphaBetaStrategy() override = default;

        ActionType play(
            const ManagerType*, 
            PlayerId, 
            uint step, 
            BoardType) override;

};

//template <class ActionType, class BoardType, class ManagerType>

template <class ActionType, class BoardType, class ManagerType>
ActionType AlphaBetaStrategy<ActionType, BoardType, ManagerType>::play(
    const ManagerType *manager,
    PlayerId id,
    uint step,
    BoardType board
) {
    if (manager.players.size() != 2)
        throw NotImplemented();

    std::vector<ActionType> actions = 
        ActionType::getActions(manager, id, step, board);

    throw NotImplemented();
};