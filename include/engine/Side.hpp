#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <optional>
#include <SFML/System/Vector2.hpp>

enum SidePieceType {Wall};
using SideVector = sf::Vector2u;

class SidePiece {
    public:
        SidePieceType pieceType;
};