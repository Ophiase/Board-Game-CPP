#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"

template <class ActionType, class BoardType, class ManagerType>
class RandomStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        RandomStrategy(const ManagerType* manager) : 
            Strategy<ActionType, BoardType, ManagerType>(manager) {};
        
        ~RandomStrategy() override = default;

        ActionType play(GameState<BoardType>) override;
};

template <class ActionType, class BoardType, class ManagerType>
ActionType RandomStrategy<ActionType, BoardType, ManagerType>::play(
    GameState<BoardType> state
) {
    srand(time(NULL));
    std::vector<ActionType> actions = 
        ActionType::getActions(this->manager, state);

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