#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <optional>

#include <engine/Cell.hpp>
#include <engine/Side.hpp>
#include <engine/Owning.hpp>

class Action;

class Board {
    protected:
        std::optional<CellPiece>** cellPieces;
    public:
        const int dimension;
        
        Board(int dimension) : dimension(dimension) {
            cellPieces = new std::optional<CellPiece>*[dimension];
            for (int i = 0; i < dimension; ++i)
                cellPieces[i] = new std::optional<CellPiece>[dimension];
        }

        ~Board() {
            for (int i = 0; i < dimension; ++i)
                delete[] cellPieces[i];
            delete[] cellPieces;
        }
        
        // ---

        virtual void applyAction(Action action);
        virtual std::vector<Action> getActions();
        
        virtual bool isFinished();
        virtual Player winner(); 

        // ---

        bool isCellEven (CellVector v);
        bool isCaseInBoard(CellVector v) const;
        bool isCaseEmpty(CellVector v) const;
        CellPiece getCellValue(CellVector v) const;
        std::optional<CellPiece> getCell(CellVector v) const;
};