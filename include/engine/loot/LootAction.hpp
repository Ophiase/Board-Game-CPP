#pragma once

#include "engine/Action.hpp"
#include "engine/Cell.hpp"

class LootAction : public Action {
    private:
    public:
        const std::vector<CellPosition> jumps;
        LootAction(PlayerId author, std::vector<CellPosition> jumps) :
        Action{author}, jumps{jumps} {};
};