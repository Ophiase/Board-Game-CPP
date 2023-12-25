#pragma once

#include "engine/type/PlayerId.hpp"

template <class ActionType, class BoardType, class ManagerType>
class Strategy {
    public:
        Strategy() {};
        virtual ~Strategy() = default;

        virtual ActionType play(
            const ManagerType*, 
            PlayerId, 
            uint step, 
            BoardType
            ) = 0;
};