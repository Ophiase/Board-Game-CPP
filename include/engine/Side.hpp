#pragma once

#include "engine/type/SidePieceType.hpp"
#include "engine/type/Piece.hpp"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <SFML/System/Vector2.hpp>
#include <tuple>

class SidePiece : public Piece {
    public:
        SidePieceType pieceType;

        SidePiece() : pieceType{SidePieceType::NoneSide} {};
        SidePiece(SidePieceType pieceType) :
            pieceType{pieceType} {};

        bool isNone() const;
        PlayerId owner() const;
        bool isQueen() const;
        /* we assume that a non empty piece is either a queen, a king or a pawn */
        bool isPawn() const;

        bool operator==(SidePiece other) const;
        bool operator==(SidePieceType other) const;
        bool operator!=(SidePiece other) const;
        bool operator!=(SidePieceType other) const;
};