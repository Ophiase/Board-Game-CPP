#pragma once

#include "graphics/screen/Screen.hpp"
#include "engine/BoardSided.hpp"

class Game : public Screen {
    private:
        sf::RectangleShape checkerBoardImage;
    public:
        sf::Vector2f const BOARD_POSITION{0.1, 0.205};
        sf::Vector2f const BOARD_SIZE{0.5, 0.5};

        Game(Launcher *launcher, std::string title="Game");

        void draw() override;

        bool mouseInsideCheckerBoard();
        bool mouseOnCase();
        bool mouseOnSide();

        CellPosition getCellPosition();
        SidePosition getSidePosition();
};