#include "engine/loot/LootAction.hpp"

std::vector<CellPosition> const LootAction::authorizedOffsets = {
    {2, 0}, {0, 2}, {-2, 0}, {0, -2},
    {2, 2}, {2, -2}, {-2, 2}, {-2, -2}
};