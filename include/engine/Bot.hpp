#pragma once

#include "utils/NotImplemented.hpp"
#include "engine/type/PlayerId.hpp"

#include <random>
#include <vector>
#include <stdexcept>
#include <unistd.h>

/*
    The bot need to be able to use

    For simplicity, we assume that bot cannot use previous computations.
    If needed we could rework MonteCarlo, so that it re-use its previous exploration.
*/
template <class ActionType, class BoardType, class ManagerType>
class Bot {
    public:
        const ManagerType *manager;
        const PlayerId botId;

        Bot(ManagerType *manager, PlayerId botId) : 
        manager{manager}, botId{botId} {}; 
        
        ActionType play(
            uint step,
            BoardType board
        );

        /*
            unsafe

            If nothing is playable, will throw an error.
        */
        ActionType play(
            PlayerId id,
            uint step,
            BoardType board
        );
};

template <class ActionType, class BoardType, class ManagerType>
ActionType Bot<ActionType, BoardType, ManagerType>::play(
    uint step,
    BoardType board
) {  
    return this->play(this->botId, step, board);
}


template <class ActionType, class BoardType, class ManagerType>
ActionType Bot<ActionType, BoardType, ManagerType>::play(
    PlayerId id,
    uint step,
    BoardType board
) {  

    srand(time(NULL));
    std::vector<ActionType> actions = 
        ActionType::getActions(manager, id, step, board);

    Cli::debug("possible actions:");
    for (auto action : actions) {
        Cli::debug("\taction: " + Cli::toString(action.jumps));
        if (action.jumps.size() > 1) {
            std::string captured = "\t\tcaptured : ";
            for (uint i = 1; i < action.jumps.size(); i++) {
                auto mid = (action.jumps[i] + action.jumps[i-1]);
                captured += board.getCell(mid).toString();
            }
            Cli::debug(captured);
        }
    }
    Cli::debug("time to act\n");

    if (actions.empty())
        throw std::invalid_argument("Cannot play any action on this board.");

    Cli::debug("?");
    int r = rand() % (actions.size());
    return actions[r];
};