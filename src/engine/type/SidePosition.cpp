#include "engine/type/SidePosition.hpp"
#include "utils/Cli.hpp"

std::string SidePosition::toString() const {
    return (this->horizontal ? "horizontal" : "vertical") + 
        Cli::toString(this->sideVector);
};