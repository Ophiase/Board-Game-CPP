#pragma once

#include "engine/type/PlayerId.hpp"
#include <vector>
#include "utils/NotImplemented.hpp"

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
        
        // ----------------------------------------------- 

        /*
            Is action authorized/correct.
        */
        virtual bool isValid(BoardType board) const = 0;
        

        /*
            If action is not valid, throw an error.
        */
        virtual BoardType apply(BoardType board) const = 0;
        
        /*
            Doe 2 actions have the same effect on the board?
        */
        bool actionEquivalence(BoardType board, Action other) const;

        // ----------------------------------------------- 

        /*
            All possibles action from current configuration (up to isomorphism).

            every class that implements Action, should have its own
            version of getActions.
        */
        static std::vector<Action> getActions(
            ManagerType *const manager, 
            PlayerId author, 
            int step,
            BoardType board);
        
        /*
            Is there any authorized/correct action ?

            every class that implements Action, should have its own
            version of hasRemainingActions.
        */
        static bool hasRemainingActions(BoardType board);
};

template <class ManagerType, class BoardType>
bool Action<ManagerType, BoardType>::actionEquivalence(BoardType board, Action other) const {
    return (this->apply(board)) == (other.apply(board));
}

template <class ManagerType, class BoardType>
std::vector<Action<ManagerType, BoardType>> 
Action<ManagerType, BoardType>::getActions(
    ManagerType *const manager, 
    PlayerId author, 
    int step,
    BoardType board) {

    throw NotImplemented();
}

template <class ManagerType, class BoardType>
bool Action<ManagerType, BoardType>::hasRemainingActions(
    BoardType board) {

    throw NotImplemented();
};