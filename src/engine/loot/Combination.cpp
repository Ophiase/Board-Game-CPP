#include "engine/loot/Combination.hpp"
#include "utils/NotImplemented.hpp"

bool Combination::isValid() const {
    /*
        The only combination we build, are made
        specificaly to be valid.
    */
    throw NotImplemented();
}

bool Combination::has(CellPosition position) const {
    for (auto x : *this)
        if (x == position)
            return true;
    return false;
}

bool Combination::operator==(Combination other) const {
    if (this->size() != other.size())
        return false;

    for (auto x : other)
        if (!this->has(x))
            return false;        

    return true;
}

bool Combination::operator!=(Combination other) const {
    return !((*this) == other);
}