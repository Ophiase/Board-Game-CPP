#pragma once

#include "Board.hpp"
#include "engine/Action.hpp"

#include <utils/NotImplemented.hpp>
#include <vector>
#include <type_traits>

template <class ActionType>
class Manager {
    static_assert(std::is_base_of<Action, ActionType>::value,
        "ActionType must inherit from Action");
    
    protected:
        std::vector<Board> configurations;
        std::vector<ActionType> actions;
        virtual Board initialBoard() = 0;
        Manager(
            std::vector<std::tuple<Player, std::string>> players) : 
            players{players} {};
    public:
        const std::vector<std::tuple<Player, std::string>> players;

        Board getConfiguration() const;
        ActionType getLastAction() const;
        Player getCurrentPlayer() const;

        virtual std::vector<ActionType> getActions() = 0;
        virtual bool canPlay(ActionType action) = 0;
        virtual void playAction(ActionType action) = 0;

        void cancel();
        int step();
};

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

template <class ActionType>
Player Manager<ActionType>::getCurrentPlayer() const {
    return this->getConfiguration().player;
};