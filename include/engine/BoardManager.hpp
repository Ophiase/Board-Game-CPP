#pragma once

#include "Board.hpp"

class BoardManager {
    std::vector<Board> configurations;
    public:
        void play();
};