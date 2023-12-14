#pragma once

#include <string>
#include "PlayerId.hpp"

class Piece {
    public:
        virtual bool isNone() const = 0;
        virtual PlayerId owner() const = 0;
};