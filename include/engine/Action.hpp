#pragma once

#include "engine/type/PlayerId.hpp"
#include <vector>
#include "utils/NotImplemented.hpp"
#include "engine/type/ScoreList.hpp"
#include "utils/Cli.hpp"

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
        virtual std::tuple<BoardType, ScoreList> apply(
            BoardType board, ScoreList) const = 0;
        
        /*
            Doe 2 actions have the same effect on the board?
        */
        bool actionEquivalence(BoardType board, Action other) const;

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
            const ManagerType *manager, 
            PlayerId author, 
            uint step,
            BoardType board);
        
        /*
            Is there any authorized/correct action ?

            every class that implements Action, should have its own
            version of hasRemainingActions.
        */
        static bool hasRemainingActions(
            const ManagerType *manager, 
            PlayerId author, 
            uint step,
            BoardType board);
};

template <class ManagerType, class BoardType>
bool Action<ManagerType, BoardType>::actionEquivalence(BoardType board, Action other) const {
    return (this->apply(board)) == (other.apply(board));
}

template <class ManagerType, class BoardType>
std::vector<Action<ManagerType, BoardType>> 
Action<ManagerType, BoardType>::getActions(
    const ManagerType *manager, 
    PlayerId author, 
    uint step,
    BoardType board) {

    throw NotImplemented();
}

template <class ManagerType, class BoardType>
bool Action<ManagerType, BoardType>::hasRemainingActions(
    const ManagerType *manager, 
    PlayerId author, 
    uint step,
    BoardType board) {

    throw NotImplemented();
};