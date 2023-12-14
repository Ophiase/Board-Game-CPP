#pragma once

#include "engine/Action.hpp"
#include "engine/Cell.hpp"

using CellPath = std::vector<CellPosition>;

class LootAction : public Action {
    private:
    public:
        static const std::vector<CellPosition> authorizedOffsets;
        
        const CellPath jumps;
        LootAction(PlayerId author, CellPath jumps) :
        Action{author}, jumps{jumps} {};
};