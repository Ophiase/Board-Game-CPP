#pragma once

template <class ActionType, class BoardType>
class Bot {
    private:
    public:
        Bot(BoardType board);
        ActionType play();
};