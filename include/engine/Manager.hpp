#pragma once

#include "Board.hpp"
#include "engine/Action.hpp"

#include <utils/NotImplemented.hpp>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include "utils/Cli.hpp"
#include "Player.hpp"
#include <climits>

using scoreList = std::vector<int>;

/*
    Generic class for game controller.
*/
template <class ActionType, class BoardType>
class Manager {
    /*
    static_assert(std::is_base_of<Action, ActionType>::value,
        "ActionType must inherit from Action");
    static_assert(std::is_base_of<Board, BoardType>::value,
        "BordType must inherit from Board");
    */

    protected:
        std::vector<scoreList> scores;
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
        std::vector<Player> getWinners() const;
        std::vector<int> const getScores() const;

        /*
            All possibles action from current configuration (up to isomorphism).
        */
        virtual std::vector<ActionType> getActions(BoardType board) const;
        std::vector<ActionType> getActions() const;
        
        /*
            Is action authorized/correct.
        */
        virtual bool canPlayAction(ActionType action) const = 0;
        
        /*
            Is there any authorized/correct action ?

            Advice: reimplement it wihout "getActions" for 
            better performances.
        */
        virtual bool canPlayAction(BoardType) const;
        bool canPlayAction() const;
        
        virtual bool isFinished(BoardType) const;
        bool isFinished() const;
        
        /*
            See the effect of the action
        */
        virtual std::tuple<BoardType, scoreList> evaluateAction(
            ActionType action, BoardType board) const = 0;
        
        std::tuple<BoardType, scoreList> evaluateAction(ActionType action) const;
    
        /*
            If action is not valid, throw an error.
        */
        void applyAction(ActionType action);
        
        /*
            Doe 2 actions have the same effect on the board?

            Advice: reimplement it wihout "evaluateAction" for 
            better performances.
        */
        virtual bool actionEquivalence(
            ActionType actionA, ActionType actionB) const;
        
        void cancel();
        uint step() const;
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
    this->scores.pop_back();
};

template <class ActionType, class BoardType>
uint Manager<ActionType, BoardType>::step() const {
    return this->actions.size();
};

template <class ActionType, class BoardType>
Player Manager<ActionType, BoardType>::getCurrentPlayer() const {
    PlayerId player = this->getConfiguration().player;
    for (uint i = 0; i < players.size(); i++)
        if (players[i] == player)
            return players[i];

    Cli::error("Current player should be defined.");
    exit(1);
}

template <class ActionType, class BoardType>
std::vector<Player> Manager<ActionType, BoardType>::getWinners() const {
    std::vector<Player> result;
    
    auto scores = this->getScores();
    int maxScore = INT_MIN;
    for (uint i = 0; i < scores.size(); i++)
        if (scores[i] > maxScore)
            maxScore = scores[i];

    for (uint i = 0; i < players.size(); i++)
        if (scores[i] == maxScore)
            result.push_back(players[i]);

    return result;
}

template <class ActionType, class BoardType>
int Manager<ActionType, BoardType>::getCurrentPlayerIndex() const {
    PlayerId player = this->getConfiguration().player;
    for (uint i = 0; i < players.size(); i++)
        if (players[i] == player)
            return i;

    Cli::error("Current player index should be defined.");
    exit(1);
}


template <class ActionType, class BoardType>
std::tuple<BoardType, scoreList> Manager<ActionType, BoardType>::evaluateAction(
    ActionType action) const {
    return this->evaluateAction(action, this->getConfiguration());
}

template <class ActionType, class BoardType>
void Manager<ActionType, BoardType>::applyAction(ActionType action) {
    if (!canPlayAction(action) || isFinished()) {
        Cli::error("Cannot play this Action");
        throw std::invalid_argument("Cannot play this Action.");
    }

    std::tuple<BoardType, scoreList> result = evaluateAction(action);

    this->configurations.push_back(std::get<0>(result));
    this->scores.push_back(std::get<1>(result));
    this->actions.push_back(action);
}

template <class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::actionEquivalence(
    ActionType actionA, ActionType actionB) const {
    
    return 
        (this->evaluateAction(actionA)) == 
        (this->evaluateAction(actionB));
}

template<class ActionType, class BoardType>
std::vector<int> const Manager<ActionType, BoardType>::getScores() const {
    return scores[scores.size()-1];
}

template<class ActionType, class BoardType>
std::vector<ActionType> Manager<ActionType, BoardType>::getActions(BoardType) const {
    throw NotImplemented();
};

template<class ActionType, class BoardType>
std::vector<ActionType> Manager<ActionType, BoardType>::getActions() const {
    return this->getActions(this->getConfiguration());
};

template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::canPlayAction(BoardType board) const {
    return this->getActions(board).size() != 0;
};

template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::canPlayAction() const {
    return canPlayAction(this->getConfiguration());
};

template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::isFinished(BoardType board) const {
    return !(this->canPlayAction(board));
};

template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::isFinished() const {
    return !(this->canPlayAction());
};