#include "engine/loot/CapturePath.hpp"
#include "utils/NotImplemented.hpp"

bool CapturePath::findCapturePath(
    CellPath &cache, CellPosition capture,
    Board const & board, CellPath _visiteds) const {

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
        if (board.isCaseInBoard(next)) {
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

    for (auto capture : this->captures)
        findCapturePath(result, capture, board);

    return result;
}