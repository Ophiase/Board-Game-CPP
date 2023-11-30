#pragma once

#include "PieceInfo.hpp"

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <SFML/System/Vector2.hpp>
#include <tuple>

using SideVector = sf::Vector2i;
class SidePosition {
    public:
        SideVector sideVector;
        bool horizontal;

        SidePosition(SideVector sideVector, bool horizontal) :
            sideVector{sideVector}, horizontal{horizontal} {};
        SidePosition(const SidePosition & sidePostion) : 
            sideVector{sidePostion.sideVector}, 
            horizontal{sidePostion.horizontal} {};
};

class SidePiece : public Piece {
    public:
        SidePieceType pieceType;

        SidePiece() : pieceType{SidePieceType::NoneSide} {};
        SidePiece(SidePieceType pieceType) :
            pieceType{pieceType} {};

        bool isNone() const;
        Player owner() const;

        bool operator==(SidePiece other);
        bool operator==(SidePieceType other);
        bool operator!=(SidePiece other);
        bool operator!=(SidePieceType other);
};