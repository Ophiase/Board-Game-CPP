#pragma once

#include <vector>
#include "engine/Action.hpp"

template <class BoardType, class ActionType>
class ExploratioNode { // TODO
    private:
        bool developped = false;
        const BoardType *state;
        const ActionType *origin;
        float score;

        ExploratioNode *parent;
        std::vector<ExploratioNode *> childs;

        /*
            ie. number of developped childs
        */
        int credence;
    public:
        ExploratioNode(); // TODO
        ~ExploratioNode(); // TODO
};