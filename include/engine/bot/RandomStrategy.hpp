#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"

template <class ActionType, class BoardType, class ManagerType>
class RandomStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        RandomStrategy() : 
            Strategy<ActionType, BoardType, ManagerType>() {};
        
        ~RandomStrategy() override = default;

        ActionType play(
            const ManagerType*, 
            PlayerId, 
            uint step, 
            BoardType) override;
};

template <class ActionType, class BoardType, class ManagerType>
ActionType RandomStrategy<ActionType, BoardType, ManagerType>::play(
    const ManagerType *manager,
    PlayerId id,
    uint step,
    BoardType board
) {
    srand(time(NULL));
    std::vector<ActionType> actions = 
        ActionType::getActions(manager, id, step, board);

    /*
    Cli::debug("possible actions:");
    for (auto action : actions) {
        Cli::debug("\taction: " + Cli::toString(action.jumps));
        if (action.jumps.size() > 1) {
            std::string captured = "\t\tcaptured : ";
            for (uint i = 1; i < action.jumps.size(); i++) {
                auto mid = (action.jumps[i] + action.jumps[i-1])/2;
                captured += board.getCell(mid).toString();
            }
            Cli::debug(captured);
        }
    }
    */
    

    if (actions.empty())
        throw std::invalid_argument("Cannot play any action on this board.");

    int r = rand() % (actions.size());
    return actions[r];
};