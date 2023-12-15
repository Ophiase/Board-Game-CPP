#pragma once

#include "utils/NotImplemented.hpp"

/*
    The bot need to be able to use

    For simplicity, we assume that bot cannot use previous computations.
    If needed we could rework MonteCarlo, so that it re-use its previous exploration.
*/
template <class ActionType, class BoardType, class ManagerType>
class Bot {
    protected:
        ManagerType *manager;

    public:
        Bot(ManagerType *manager
            // maybe add lambda for metrics
        ) : manager{manager} {};
        
        ActionType play();
};

template <class ActionType, class BoardType, class ManagerType>
ActionType Bot<ActionType, BoardType, ManagerType>::play() {  
    throw NotImplemented();
};