#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <optional>
#include <SFML/System/Vector2.hpp>

enum CellPieceType {Pawn, PawnBis};
using CellVector = sf::Vector2i;

class CellPiece {
    public:
        CellPieceType pieceType;
};