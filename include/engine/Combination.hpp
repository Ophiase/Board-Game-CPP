#pragma once

#include <vector>
#include <engine/Board.hpp>

/*
    A Combination is a set of CellPositions.
    If it is requiered in the future, we could make combination a generic type.

    Two combinations should be considered equals if they share
    the same elements (even wihout same order).

    We admit that a combination don't have twice the same elements
*/

//using Combination = std::vector<CellPosition>;

class Combination : public std::vector<CellPosition> {
    public:
        using std::vector<CellPosition>::vector;
        
        bool operator==(Combination other) const;
        bool operator!=(Combination other) const;
};