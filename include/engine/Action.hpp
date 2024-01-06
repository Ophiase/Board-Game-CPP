#pragma once

#include "engine/type/PlayerId.hpp"
#include <vector>
#include "utils/NotImplemented.hpp"
#include "engine/type/ScoreList.hpp"
#include "utils/Cli.hpp"
#include "engine/GameState.hpp"

/*
    PlaceHolder, game defined.
*/
template <class ManagerType, class BoardType>
class Action {
    private:
    public:
        const ManagerType *manager;
        const PlayerId author;
        const uint step;

        Action(const ManagerType * manager, PlayerId author, uint step) : 
        manager{manager}, author{author}, step{step} {};

        Action (const Action & other) :
        manager{other.manager}, author{other.author}, step{other.step} {};
        
        // ----------------------------------------------- 

        /*
            Is action authorized/correct.
        */
        virtual bool isValid(GameState<BoardType>) const = 0;
        
        /*
            If action is not valid, throw an error.
        */
        virtual GameState<BoardType> apply(GameState<BoardType>) const = 0;
        
        /*
            Doe 2 actions have the same effect on the board?
        */
        bool actionEquivalence(GameState<BoardType>, const Action &other) const;

        /*
            for debug purpose.
        */
        virtual std::string toString() const = 0;

        // ----------------------------------------------- 

        /*
            All possibles action from current configuration (up to isomorphism).

            every class that implements Action, should have its own
            version of getActions.
        */
        static std::vector<Action> getActions(
            const ManagerType *manager, GameState<BoardType>);
        
        /*
            Is there any authorized/correct action ?

            every class that implements Action, should have its own
            version of hasRemainingActions.
        */
        static bool hasRemainingActions(
            const ManagerType *manager, GameState<BoardType>);
};

template <class ManagerType, class BoardType>
bool Action<ManagerType, BoardType>::actionEquivalence(GameState<BoardType> gamestate, const Action & other) const {
    return (this->apply(gamestate)) == (other.apply(gamestate));
}

template <class ManagerType, class BoardType>
std::vector<Action<ManagerType, BoardType>> 
Action<ManagerType, BoardType>::getActions(
    const ManagerType *, GameState<BoardType>) {
    throw NotImplemented();
}

template <class ManagerType, class BoardType>
bool Action<ManagerType, BoardType>::hasRemainingActions(
    const ManagerType *manager, GameState<BoardType> state) {

    return !getActions(manager, state).empty();
};