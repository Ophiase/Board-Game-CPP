#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"

#include "ExplorationNode.hpp"
#include "Score.hpp"

template <class ActionType, class BoardType, class ManagerType>
class MonteCarloStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        int maxHeight;

        MonteCarloStrategy(const ManagerType* manager, int maxHeight=20) : 
            Strategy<ActionType, BoardType, ManagerType>(manager), maxHeight{maxHeight} {};
        
        ~MonteCarloStrategy() override = default;

        ActionType play(GameState<BoardType>) override;

        // needs a tree representation of exploration (ie. Exploration Node)
        // needs a method to choose which node to explore
        // needs a method to random walk a node, state -> final score
            // needs a function final score -> value (ie. Score::idToRelativeScorePosition)
        // needs a method to update tree
};

template <class ActionType, class BoardType, class ManagerType>
ActionType MonteCarloStrategy<ActionType, BoardType, ManagerType>::play(
    GameState<BoardType> state
) {
    srand(time(NULL));
    std::vector<ActionType> actions = 
        ActionType::getActions(this->manager, state);

    /*
        AlphaBetaStrategy for 1v1 and Greedy for n players already performs well.
        It's currently not necessary to implements MonteCarlo.
    */
    throw NotImplemented();
};