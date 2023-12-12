#pragma once

#include "engine/Action.hpp"
#include "engine/Cell.hpp"

using CellPath = std::vector<CellPosition>;

class LootAction : public Action {
    private:
    public:
        const CellPath jumps;
        LootAction(PlayerId author, CellPath jumps) :
        Action{author}, jumps{jumps} {};
};