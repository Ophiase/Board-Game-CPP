#pragma once

#include "NotImplemented.hpp"

/*
    The bot need to be able to use

    ActionType::getActions
    action.apply
*/
template <class ActionType, class BoardType, class ManagerType>
class Bot {
    protected:
        ManagerType *manager;
    public:
        Bot(ManagerType *manager) : manager{manager} {};
        void update(
            ActionType action, 
            BoardType board);
        ActionType play();
};

template <class ActionType, class BoardType>
void Bot<ActionType, BoardType>::update(
    ActionType action, BoardType board) {

    throw notImplemented();
};

template <class ActionType, class BoardType>
void Bot<ActionType, BoardType>::play() {
    throw notImplemented();
};