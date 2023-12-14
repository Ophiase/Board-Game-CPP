#pragma once

#include "Combination.hpp"
#include "LootAction.hpp"

class CapturePath {
    public:
        Combination captures;
        Combination visiteds;

        CapturePath(Combination captures, Combination visiteds) :
        captures{captures}, visiteds{visiteds} {};

        CapturePath(CapturePath const & other) : 
        captures{other.captures}, visiteds{other.visiteds} {};

        CellPath toCellpath() const;
};