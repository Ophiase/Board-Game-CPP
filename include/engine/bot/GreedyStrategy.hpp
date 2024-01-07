#pragma once

#include "Strategy.hpp"
#include "utils/NotImplemented.hpp"
#include "Score.hpp"

#include <limits>
#include <algorithm>
#include <vector>

template <class ActionType, class BoardType, class ManagerType>
class GreedyStrategy : 
public Strategy<ActionType, BoardType, ManagerType> {
    public:
        GreedyStrategy(const ManagerType* manager) : 
            Strategy<ActionType, BoardType, ManagerType>(manager) {};
        
        ~GreedyStrategy() override = default;

        ActionType play(GameState<BoardType>) override;
};

template <class ActionType, class BoardType, class ManagerType>
ActionType GreedyStrategy<ActionType, BoardType, ManagerType>::play(
    GameState<BoardType> state
) {
    srand(time(NULL));

    std::vector<ActionType> actions = 
        ActionType::getActions(this->manager, state);

    if (actions.empty())
        throw std::invalid_argument("Cannot play any action on this board.");

    // Compute scores and best score

    std::vector<float> scores{};
    float bestScore = std::numeric_limits<float>::min();
    for (ActionType & action : actions) {
        ScoreList scoreList = action.apply(state).scores;
        float currentScore = Score::idToRelativeScorePosition(scoreList, state.player);
        scores.push_back(currentScore);
        bestScore = std::max(currentScore, bestScore);
    }

    // Compute best actions

    const float EPSILON = 10e-3;
    
    std::vector<ActionType*> bestActions;
    for (uint i = 0; i < actions.size(); i++)
        if (std::fabs(scores[i] - bestScore) < EPSILON)
            bestActions.push_back(&actions[i]);

    // Choose a random best action

    int r = rand() % (bestActions.size());
    return *bestActions[r];
};