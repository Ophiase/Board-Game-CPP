#pragma once

#include <vector>
#include <engine/Board.hpp>

/*
    A Combination is a set of CellPositions.

    We could use a std::set to achieve this, but for
    our implementation of LootManager it would be less optimized
    because our n will be very low. 
    In this very specific case std::vector is faster.

    Two combinations should be considered equals if they share
    the same elements (even wihout same order).

    We admit that a combination don't have twice the same elements
*/

//using Combination = std::vector<CellPosition>;

class Combination : public std::vector<CellPosition> {
    public:
        using std::vector<CellPosition>::vector;

        /*
            Check if there is not two time the same value.
            O(n²)
        */
        bool isValid() const;
        
        /*
            O(n)
        */
        bool has(CellPosition position) const;

        /*
            Require the two combinations to be valid.
            O(n²) but for our usage n are low.
        */
        bool operator==(Combination other) const;
        bool operator!=(Combination other) const;
};