#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "engine/Cell.hpp"
#include "engine/Side.hpp"

/*
    Base class for any game board.
    Should be immutable.
*/
class Board {
        const std::vector<std::vector<CellPiece>> cellPieces;
        const Player player{0};
    
    protected:
        // Sub class const init
        Board(std::vector<std::vector<CellPiece>> cellPieces, Player player) : 
            cellPieces{cellPieces}, player{player} {};
    public:
        Board(const Board &board) : 
            Board{board.cellPieces, board.player} {};

        /* 
            Should be overridden.
            Require for all n :
            - Board(n) respect game init board structure
            - implementeation constructed with Board(cellPieces), 
                ie.
                - assert(Board(n).getDimension() == n)
                - assert(Board(n).step == 0)
                - assert(Board(n).lastBoard == std::nulopt)
                - assert(Board(n).lastAction == std::nulopt)
        */
        Board(int dimension);
        ~Board();

        // ---------------------------------------------------
        
        int getDimension() const ;

        // ---------------------------------------------------

        bool isCellEven (CellPosition v) const;
        bool isCaseInBoard(CellPosition v) const;
        bool isCaseEmpty(CellPosition v) const;
        CellPiece getCell(CellPosition v) const;
};