#include "engine/loot/CapturePath.hpp"
#include "utils/NotImplemented.hpp"
#include "engine/loot/LootAction.hpp"

bool CapturePath::findCapturePath(
    CellPath &cache, CellPosition capture,
    Board const & board, Combination _visiteds) const {
    
    auto current = cache[cache.size() - 1];
    _visiteds.push_back(current);

    if (_visiteds.size() > 1) {
        auto last = cache[cache.size() - 2];
        auto mid = (current + last) / 2;
        if (capture == mid)
            return true;
    }

    for (auto offset : LootAction::authorizedOffsets) {
        auto next = current + offset;
        auto mid = current + (offset/2);
        if (
            board.isCaseInBoard(next) && 
            !board.isCaseEmpty(mid) &&
            visiteds.has(next) &&
            !_visiteds.has(next)
        ) {
            cache.push_back(next);
            if (findCapturePath(cache, capture, board, _visiteds))
                return true;
            cache.pop_back();
        }
    }

    _visiteds.pop_back();
    return false;
}

CellPath CapturePath::toCellPath(Board const & board) const {
    CellPath result{this->visiteds[0]};
    
    for (auto capture : this->captures) {
        if (!findCapturePath(result, capture, board))
            std::logic_error("Should find capture path.");
    }
    return result;
}