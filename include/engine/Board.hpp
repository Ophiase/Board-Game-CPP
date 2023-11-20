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

        bool isCellEven (CellVector v) {
            return ((v.x + v.y) & 1) == 0;
        }

        bool isCaseInBoard(CellVector v) const {
            return (0 <= v.x) && (0 <= v.y)
                && (v.x < dimension) && (v.y < dimension);
        }
        bool isCaseEmpty(CellVector v) const {
            return !getCell(v).has_value();
        }
        CellPiece getCellValue(CellVector v) const {
            return getCell(v).value();
        }
        std::optional<CellPiece> getCell(CellVector v) const {
            return cellPieces[v.y][v.x];
        }
};

class BoardSided : Board {
    protected :
        SidePiece** horizontalSidePieces;
        SidePiece** verticalSidePieces;
    public:
        BoardSided(int dimension) : Board(dimension) {
            horizontalSidePieces = new SidePiece*[dimension + 1];
            for (int i = 0; i <= dimension; ++i)
                horizontalSidePieces[i] = new SidePiece[dimension];

            verticalSidePieces = new SidePiece*[dimension];
            for (int i = 0; i < dimension; ++i)
                verticalSidePieces[i] = new SidePiece[dimension + 1];
        }
        
        ~BoardSided() {
            for (int i = 0; i <= dimension; ++i)
                delete[] horizontalSidePieces[i];
            delete[] horizontalSidePieces;

            for (int i = 0; i < dimension; ++i)
                delete[] verticalSidePieces[i];
            delete[] verticalSidePieces;
        }
};