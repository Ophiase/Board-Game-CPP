#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <optional>
#include <SFML/System/Vector2.hpp>
#include "PieceInfo.hpp"
#include "tuple"

using SideVector = std::tuple<sf::Vector2i, bool>;
class SidePiece {
    public:
        SidePieceType pieceType;
        Player owner();
};