#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <SFML/System/Vector2.hpp>

#include "engine/type/Piece.hpp"
#include "engine/type/CellPieceType.hpp"

class CellPiece : public Piece {
    public:
        CellPieceType pieceType;
        
        CellPiece() : pieceType{CellPieceType::NoneCell} {};
        CellPiece(CellPieceType pieceType) :
            pieceType{pieceType} {};
        
        bool isNone() const;
        PlayerId owner() const;

        bool operator==(CellPiece other) const;
        bool operator==(CellPieceType other) const;
        bool operator!=(CellPiece other) const;
        bool operator!=(CellPieceType other) const;

        std::string toString() const;
};