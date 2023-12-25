#pragma once

#include "utils/NotImplemented.hpp"
#include "engine/type/PlayerId.hpp"

#include "Strategy.hpp"
#include "RandomStrategy.hpp"

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
        Strategy<ActionType, BoardType, ManagerType> *strategy;

        Bot(ManagerType *manager, PlayerId botId, 
        Strategy<ActionType, BoardType, ManagerType> *strategy = nullptr); 
        ~Bot();
        
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
Bot<ActionType, BoardType, ManagerType>::Bot(
    ManagerType *manager, PlayerId botId, 
    Strategy<ActionType, BoardType, ManagerType> *strategy
    ) : manager{manager}, botId{botId} 
{
    this->strategy = strategy ? 
        strategy :
        new RandomStrategy<ActionType, BoardType, ManagerType>();
};

template <class ActionType, class BoardType, class ManagerType>
Bot<ActionType, BoardType, ManagerType>::~Bot() {
    delete this->strategy;
}

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
    return this->strategy->play(
        this->manager, 
        id, 
        step, 
        board);
};