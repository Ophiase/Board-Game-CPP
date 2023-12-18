#pragma once

#include "engine/type/PlayerId.hpp"
#include <string>

class Player {
    public:
        const PlayerId id;
        const std::string name;
        const bool isAI;

        Player(PlayerId id, std::string name, bool isAI = false) : 
        id{id}, name{name}, isAI{isAI} {};

        Player(Player const &other) : 
        Player{other.id, other.name, other.isAI} {};

        bool operator==(PlayerId otherId) const;
        bool operator==(Player other) const;
        bool operator!=(PlayerId otherId) const;
        bool operator!=(Player other) const;
};