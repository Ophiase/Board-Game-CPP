#pragma once

#include "SFML/Graphics.hpp"

using SideVector = sf::Vector2i;
class SidePosition {
    public:
        SideVector sideVector;
        bool horizontal;

        SidePosition(SideVector sideVector, bool horizontal) :
            sideVector{sideVector}, horizontal{horizontal} {};
        SidePosition(const SidePosition & sidePostion) : 
            sideVector{sidePostion.sideVector}, 
            horizontal{sidePostion.horizontal} {};

        SidePosition& operator=(const SidePosition& other);
        bool operator==(const SidePosition &) const;
        bool operator!=(const SidePosition &) const;

        std::string toString() const;
};