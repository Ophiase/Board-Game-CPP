#pragma once

#include "Board.hpp"

class BoardSided : public Board {
    protected :
        SidePiece** horizontalSidePieces;
        SidePiece** verticalSidePieces;
    public:
        BoardSided(int dimension);
        ~BoardSided();
};