#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <SFML/System/Vector2.hpp>

#include "PieceInfo.hpp"

using CellPosition = sf::Vector2i;
class CellPiece : public Piece {
    public:
        CellPieceType pieceType;
        
        CellPiece() : pieceType{CellPieceType::NoneCell} {};
        CellPiece(CellPieceType pieceType) :
            pieceType{pieceType} {};
        
        bool isNone() const;
        Player owner() const;

        bool operator==(CellPiece other) const;
        bool operator==(CellPieceType other) const;
        bool operator!=(CellPiece other) const;
        bool operator!=(CellPieceType other) const;
};