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
        bool isQueen() const;
        bool isKing() const;
        /* we assume that a non empty piece is either a queen, a king or a pawn */
        bool isPawn() const;

        bool isOwner(PlayerId) const;

        bool operator==(CellPiece other) const;
        bool operator==(CellPieceType other) const;
        bool operator!=(CellPiece other) const;
        bool operator!=(CellPieceType other) const;

        std::string toString() const;
};