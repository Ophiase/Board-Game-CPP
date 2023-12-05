#pragma once

#include "Board.hpp"
#include "engine/Action.hpp"

#include <utils/NotImplemented.hpp>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include "utils/Cli.hpp"
#include "Player.hpp"

template <class ActionType, class BoardType>
class Manager {
    static_assert(std::is_base_of<Action, ActionType>::value,
        "ActionType must inherit from Action");
    static_assert(std::is_base_of<Board, BoardType>::value,
        "BordType must inherit from Board");

    protected:
        std::vector<BoardType> configurations;
        std::vector<ActionType> actions;
        virtual BoardType initialBoard() = 0;
        Manager(
            std::vector<Player> players) : 
            players{players} {};

        int getCurrentPlayerIndex() const;
    public:
        const std::vector<Player> players;

        BoardType getConfiguration() const;
        ActionType getLastAction() const;
        Player getCurrentPlayer() const;
        
        /*
            All possibles action from current configuration (up to isomorphism).
        */
        virtual std::vector<ActionType> getActions() const = 0;
        
        /*
            Is action authorized/correct.
        */
        virtual bool canPlay(ActionType action) const = 0;
        
        /*
            See the effect of the action
        */
        virtual BoardType evaluateAction(
            ActionType action, BoardType board) const = 0;
        
        BoardType evaluateAction(ActionType action) const;
        void applyAction(ActionType action);
        bool actionEquivalence(
            ActionType actionA, ActionType actionB) const;
        void cancel();
        int step() const;
};

template <class ActionType, class BoardType>
BoardType Manager<ActionType, BoardType>::getConfiguration() const {
    return configurations.at(configurations.size()-1);
};

template <class ActionType, class BoardType>
ActionType Manager<ActionType, BoardType>::getLastAction() const {
    return actions.at(actions.size() -1);
};

template <class ActionType, class BoardType>
void Manager<ActionType, BoardType>::cancel() {
    this->configurations.pop_back();
    this->actions.pop_back();
};

template <class ActionType, class BoardType>
int Manager<ActionType, BoardType>::step() const {
    return this->actions.size();
};

template <class ActionType, class BoardType>
Player Manager<ActionType, BoardType>::getCurrentPlayer() const {
    PlayerId player = this->getConfiguration().player;
    for (uint i = 0; i < players.size(); i++)
        if (players[i] == player)
            return players[i];

    Cli::warning("Current player should be defined.");
    exit(1);
}

template <class ActionType, class BoardType>
int Manager<ActionType, BoardType>::getCurrentPlayerIndex() const {
    PlayerId player = this->getConfiguration().player;
    for (uint i = 0; i < players.size(); i++)
        if (players[i] == player)
            return i;

    Cli::warning("Current player index should be defined.");
    exit(1);
}


template <class ActionType, class BoardType>
BoardType Manager<ActionType, BoardType>::evaluateAction(ActionType action) const {
    return this->evaluateAction(action, this->getConfiguration());
}

template <class ActionType, class BoardType>
void Manager<ActionType, BoardType>::applyAction(ActionType action) {
    this->configurations.push_back(evaluateAction(action));
    this->actions.push_back(action);
}

template <class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::actionEquivalence(
    ActionType actionA, ActionType actionB) const {

    return 
        (this->evaluateAction(actionA)) == 
        (this->evaluateAction(actionB));
}