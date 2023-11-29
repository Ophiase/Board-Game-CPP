#pragma once

#include "graphics/screen/Screen.hpp"
#include "engine/BoardSided.hpp"
#include "graphics/screen/Text.hpp"
#include "graphics/screen/TextButton.hpp"
#include "ResourcesLoader.hpp"

class Game : public Screen {
    private:
        sf::RenderTexture checkBoardTexture;
        sf::RectangleShape checkerBoardImage;

        Text currentPlayer;
        TextButton exitButton;
    protected:
        void setCurrentPlayer(std::string currentPlayer);
    public:
        sf::Vector2f const BOARD_POSITION{0.1, 0.205};
        sf::Vector2f const BOARD_SIZE{0.5, 0.5};

        Game(Launcher *launcher, std::string title="Game");

        void updateBoard();
        void updateBoardContent(Board board);
        void updateBoardSidedContent(BoardSided boardSided);

        void draw() override;

        bool mouseInsideCheckerBoard();
        bool mouseOnCase();
        bool mouseOnSide();

        CellPosition getCellPosition();
        SidePosition getSidePosition();
};