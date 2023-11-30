#pragma once

#include "Board.hpp"
#include "engine/Action.hpp"

#include <utils/NotImplemented.hpp>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include "utils/Cli.hpp"

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
        std::string getCurrentPlayerName() const;

        /*
            All possibles action from current configuration (up to isomorphism).
        */
        virtual std::vector<ActionType> getActions() const = 0;
        virtual bool canPlay(ActionType action) const = 0;
        virtual void playAction(ActionType action) = 0;
        virtual bool actionEquivalence(
            ActionType actionA, ActionType actionB) const = 0;

        void cancel();
        int step() const;
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
int Manager<ActionType>::step() const {
    return this->actions.size();
};

template <class ActionType>
Player Manager<ActionType>::getCurrentPlayer() const {
    return this->getConfiguration().player;
};

template <class ActionType>
std::string Manager<ActionType>::getCurrentPlayerName() const {
    Player player = this->getConfiguration().player;
    for (auto candidate : players)
        if (std::get<0>(candidate) == player)
            return std::get<1>(candidate);

    Cli::warning("CurrentPlayerName should be defined.");
    exit(1);
};