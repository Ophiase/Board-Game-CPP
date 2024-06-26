#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "engine/Cell.hpp"
#include "engine/Side.hpp"
#include "engine/type/CellPosition.hpp"

/*
    Base class for any board.

    Should be immutable.
*/
class Board {
    public:
        const std::vector<std::vector<CellPiece>> cellPieces;
        const PlayerId player{0};
        
        Board(std::vector<std::vector<CellPiece>> cellPieces, PlayerId player) : 
            cellPieces{cellPieces}, player{player} {};
        Board(const Board &board) : 
            Board{board.cellPieces, board.player} {};

        virtual bool operator==(const Board &) const;
        virtual bool operator!=(const Board &) const;

        // ---------------------------------------------------
        
        uint getDimension() const ;

        // ---------------------------------------------------

        bool isCellEven (CellPosition v) const;
        bool isCellEven (int x, int y) const;
        bool isCaseInBoard(CellPosition v) const;
        bool isCaseEmpty(CellPosition v) const;
        CellPiece getCell(CellPosition v) const;
        CellPiece getCell(int x, int y) const;

        std::string toString() const;
};