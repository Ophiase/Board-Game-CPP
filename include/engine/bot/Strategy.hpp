#pragma once

#include "engine/type/PlayerId.hpp"
#include "engine/GameState.hpp"

template <class ActionType, class BoardType, class ManagerType>
class Strategy {
    public:
        const ManagerType* manager;

        Strategy(const ManagerType* manager) : manager{manager} {};
        virtual ~Strategy() = default;

        virtual ActionType play(GameState<BoardType>) = 0;
};