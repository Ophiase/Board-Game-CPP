#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "engine/Cell.hpp"
#include "engine/Side.hpp"
#include "engine/Action.hpp"

/*
    Base class for any game board.
    Should be immutable.
*/
class Board {
    public:
        const std::vector<std::vector<CellPiece>> cellPieces;
        const Player player{0};
    
    protected:
        // Sub class const init
        Board(std::vector<std::vector<CellPiece>> cellPieces, Player player) : 
            cellPieces{cellPieces}, player{player} {};
    public:
        /*
            The board copy is for simulation purpose.
            It creates a new initial Board in same configuration as the board passed in argument.
        */
        Board(const Board &);

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

        virtual bool isValidAction(Action action);
        virtual Board applyAction(Action action);
        virtual Board cancelAction();
        virtual const std::vector<Action> & getActions();
        
        // ---------------------------------------------------
        
        int getDimension() const ;
        virtual bool isFinished();
        virtual Player winner(); 

        // ---------------------------------------------------

        bool isCellEven (CellPosition v) const;
        bool isCaseInBoard(CellPosition v) const;
        bool isCaseEmpty(CellPosition v) const;
        CellPiece getCell(CellPosition v) const;
};