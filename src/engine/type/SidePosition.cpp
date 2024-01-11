#include "engine/type/SidePosition.hpp"
#include "utils/Cli.hpp"

std::string SidePosition::toString() const {
    return (this->horizontal ? "h - " : "v - ") + 
        Cli::toString(this->sideVector);
};

SidePosition& SidePosition::operator=(const SidePosition& other) {
    this->sideVector = other.sideVector;
    this->horizontal = other.horizontal;
    return *this;
}

bool SidePosition::operator==(const SidePosition & other) const {
    return (other.horizontal == this->horizontal) && (other.sideVector == this->sideVector);
}

bool SidePosition::operator!=(const SidePosition & other) const {
    return !(*this == other);
}
