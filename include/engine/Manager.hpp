#pragma once

#include "Board.hpp"
#include "engine/Action.hpp"
#include "engine/type/ScoreList.hpp"
#include "engine/GameState.hpp"

#include <utils/NotImplemented.hpp>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include "utils/Cli.hpp"
#include "Player.hpp"
#include <climits>


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
        std::vector<GameState<BoardType>> states;
        std::vector<ActionType> actions;
        virtual BoardType initialBoard() = 0;
        Manager(
            std::vector<Player> players) : 
            players{players} {};

    public:
        const std::vector<Player> players;

        GameState<BoardType> getState() const;

        BoardType getBoard() const;
        ActionType getLastAction() const;
        Player getCurrentPlayer() const;
        int getPlayerIndex(PlayerId) const;
        int getCurrentPlayerIndex() const;
        ScoreList const getScores() const;
        
        std::vector<Player> getWinners() const;

        /*
            All possibles action from current configuration (up to isomorphism).
        */
        std::vector<ActionType> getActions(GameState<BoardType>) const;
        std::vector<ActionType> getActions() const;
        
        /*
            Is action authorized/correct.
        */
        bool canPlayAction(ActionType action) const;
        
        /*
            Is there any authorized/correct action ?

            Need to be reimplemented in child class using 
            ActionType::hasRemainingAction
        */
        virtual bool canPlayAction(GameState<BoardType>) const;
        bool canPlayAction() const;
        
        bool isFinished(GameState<BoardType>) const;
        bool isFinished() const;
        
        /*
            See the effect of the action
        */
        GameState<BoardType> evaluateAction(
            ActionType action, GameState<BoardType>) const;
        
        GameState<BoardType> evaluateAction(ActionType action) const;
    
        /*
            If action is not valid, throw an error.
        */
        void applyAction(ActionType action);
        
        /*
            Doe 2 actions have the same effect on the board?
        */
        bool actionEquivalence(
            ActionType actionA, ActionType actionB) const;
        
        void cancel();
        uint step() const;
};


template <class ActionType, class BoardType>
GameState<BoardType> Manager<ActionType, BoardType>::getState() const {
    return states[states.size()-1];
};

template <class ActionType, class BoardType>
BoardType Manager<ActionType, BoardType>::getBoard() const {
    return getState().board;
};

template <class ActionType, class BoardType>
ActionType Manager<ActionType, BoardType>::getLastAction() const {
    return actions.at(actions.size() -1);
};

template <class ActionType, class BoardType>
void Manager<ActionType, BoardType>::cancel() {
    this->states.pop_back();
    this->actions.pop_back();
};

template <class ActionType, class BoardType>
uint Manager<ActionType, BoardType>::step() const {
    return this->getState().step;
};

template <class ActionType, class BoardType>
Player Manager<ActionType, BoardType>::getCurrentPlayer() const {
    PlayerId player = this->getState().player;
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
int Manager<ActionType, BoardType>::getPlayerIndex(PlayerId id) const {
    for (uint i = 0; i < players.size(); i++)
        if (players[i] == id)
            return i;

    Cli::error("Player index should be defined.");
    exit(1);
}

template <class ActionType, class BoardType>
int Manager<ActionType, BoardType>::getCurrentPlayerIndex() const {
    return this->getPlayerIndex(this->getState().player);
}


template <class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::canPlayAction(ActionType action) const {
    return action.isValid(this->getState());
}

template <class ActionType, class BoardType>
GameState<BoardType> Manager<ActionType, BoardType>::evaluateAction(
    ActionType action, GameState<BoardType> state) const {
    
    return action.apply(state);
}

template <class ActionType, class BoardType>
GameState<BoardType> Manager<ActionType, BoardType>::evaluateAction(
    ActionType action) const {
    return this->evaluateAction(action, this->getState());
}

template <class ActionType, class BoardType>
void Manager<ActionType, BoardType>::applyAction(ActionType action) {
    if (!canPlayAction(action) || isFinished()) {
        Cli::error("Cannot play this Action");
        Cli::error("Action : " + action.toString());
        Cli::error("On Board :\n" + this->getBoard().toString());

        throw std::invalid_argument("Cannot play this Action.");
    }

    GameState<BoardType> result = evaluateAction(action);

    this->states.push_back(result);
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
ScoreList const Manager<ActionType, BoardType>::getScores() const {
    return this->getState().scores;
}

template<class ActionType, class BoardType>
std::vector<ActionType> Manager<ActionType, BoardType>::
getActions(GameState<BoardType>) const {
    
    throw NotImplemented(); 
    
    // needs to be reimplemented in child class because "this"
    // need to be correctly casted

    // return ActionType::getActions(this, state);
    
};

template<class ActionType, class BoardType>
std::vector<ActionType> Manager<ActionType, BoardType>::getActions() const {
    return this->getActions(this->getBoard());
};


template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::canPlayAction(GameState<BoardType>) const {

    throw NotImplemented();
    
    // needs to be reimplemented in child class because "this"
    // need to be correctly casted

    // return ActionType::hasRemainingActions(this, state);
    
};

template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::canPlayAction() const {
    return canPlayAction(getState());
};

template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::isFinished(GameState<BoardType> state) const {
    return !(this->canPlayAction(state));
};

template<class ActionType, class BoardType>
bool Manager<ActionType, BoardType>::isFinished() const {
    return !(this->canPlayAction());
};