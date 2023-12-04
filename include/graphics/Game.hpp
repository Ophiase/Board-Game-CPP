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
        Text score;
        TextButton exitButton;
    protected:
        void setCurrentPlayer(std::string currentPlayer);
        void setScores(std::vector<float> scores);
        void setScores(std::vector<int> scores);

        /*
            Position Inside The cell (centered toward the origin).
            Returns values between (-0.5, -0.5) and (0.5, 0.5)
        */
        sf::Vector2f insideCellPosition() const;
    public:
        sf::Vector2f const BOARD_POSITION{0.1, 0.205};
        sf::Vector2f const BOARD_SIZE{0.5, 0.5};

        Game(Launcher *launcher, std::string title="Game");

        void updateBoard();
        void updateBoardContent(Board board);
        void updateBoardSidedContent(BoardSided boardSided);

        void draw() override;

        sf::Vector2f mouseBoardPosition() const;
        bool mouseInsideCheckerBoard() const;

        /* if extended it will not check that the case is inside the board */
        bool mouseOnCase(bool extended=false) const;
        /* if extended it will not check that the case is inside the board */
        bool mouseOnSide(bool extended=false) const;

        CellPosition getCellPosition() const;
        SidePosition getSidePosition() const;
};