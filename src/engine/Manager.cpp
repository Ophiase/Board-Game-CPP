#include "engine/Manager.hpp"

template <class ActionType>
Board Manager<ActionType>::getConfiguration() const {
    return configurations.at(configurations.size()-1);
};

template <class ActionType>
ActionType Manager<ActionType>::getLastAction() const {
    return actions.at(actions.size() -1);
};

template <class ActionType>
void Manager<ActionType>::cancel() {
    this->configurations.pop_back();
    this->actions.pop_back();
};

template <class ActionType>
int Manager<ActionType>::step() {
    return this->actions.size();
};