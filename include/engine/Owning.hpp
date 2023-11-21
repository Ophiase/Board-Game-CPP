#pragma once

#include "PieceInfo.hpp"
#include "Side.hpp"
#include "Cell.hpp"

class Owning {
    public:
        Owning() = delete;
        static Player owner(CellPieceType piece);
        static Player owner(SidePieceType piece);
};
