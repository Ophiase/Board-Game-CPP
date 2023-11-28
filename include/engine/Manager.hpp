#pragma once


#include "Board.hpp"
#include "engine/Action.hpp"

#include <vector>
#include <type_traits>

template <class ActionType>
class Manager {
    static_assert(std::is_base_of<Action, ActionType>::value,
        "ActionType must inherit from Action");
    
    protected:
        std::vector<Board> configurations;
        std::vector<ActionType> actions;
    public:
        Manager<ActionType>(Board initialBoard) : 
        configurations{std::vector<Board>{initialBoard}} {};

        Board getConfiguration() const;
        ActionType getLastAction() const;

        virtual std::vector<ActionType> getActions();
        virtual bool canPlay(ActionType action);
        virtual void playAction(ActionType action);

        void cancel();
        int step();
};