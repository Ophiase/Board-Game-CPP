#pragma once

#include "type/ScoreList.hpp"
#include "type/PlayerId.hpp"

#include <iostream>

template <class BoardType>
class GameState {
    public:
        const BoardType board;
        const ScoreList scores;
        const uint step;
        const PlayerId player;

        GameState(
            BoardType board, ScoreList scores, 
            uint step = 0, PlayerId player = 0) :
        board{board}, scores{scores}, step{step}, player{player} {};

        bool operator==(const GameState& other) const {
            return this->board == other.board;
        }

        bool operator!=(const GameState& other) const {
            return !(*this == other);
        }
};