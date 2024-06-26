#pragma once

#include "utils/NotImplemented.hpp"
#include "graphics/screen/Screen.hpp"
#include "engine/BoardSided.hpp"
#include "graphics/screen/Text.hpp"
#include "graphics/screen/TextButton.hpp"
#include "ResourcesLoader.hpp"

/*
    Parent class for Game View
*/
class Game : public Screen {
    private:
        sf::RectangleShape checkerBoardImage;
        Text currentPlayer;
        Text score;
        TextButton exitButton;
        Text message;
    protected:
        sf::RenderTexture checkBoardTexture;

        void setCurrentPlayer(std::string currentPlayer);
        void setScores(std::vector<float> scores);
        void setScores(std::vector<int> scores);

        /*
            Position Inside The cell (centered toward the origin).
            Returns values between (-0.5, -0.5) and (0.5, 0.5)
        */
        sf::Vector2f insideCellPosition(Board) const;

        /*
            Reset interface on begining of a turn.
        */
        virtual void startTurn();

        /*
            If cache is empty, it cancel the last action,
            otherwise it empty the cache. 
        */
        virtual void cancelAction();

        /*
            Launch the action in cache.
        */
        virtual void playAction();

        /*
            Initialize the bots.
        */
        virtual void AIinit();

        /*
            Delegate turn to an AI.
        */
        virtual void AIturn();

        /*
            Can the user interact with the game ? 
        */
        bool interactive{false};
        bool isFinished{false};

    public:
        sf::Vector2f const BOARD_POSITION{0.1, 0.24};
        sf::Vector2f const BOARD_SIZE{0.5, 0.5};
        
        /*
            Percentage of a cell that is considered as cell (instead of side).
        */
        float const CELL_TRESHOLD;

        Game(Launcher *launcher, std::string title="Game", 
            float CELL_TRESHOLD=0.7);

        void updateBoard(Board board);
        void updateBoardContent(Board board);
        void updateBoardSidedContent(BoardSided boardSided);

        void draw() override;

        sf::Vector2f mouseBoardPosition() const;
        bool mouseInsideCheckerBoard() const;

        /* if extended it will not check that the case is inside the board */
        bool mouseOnCase(Board board, bool extended=false) const;
        /* if extended it will not check that the case is inside the board */
        bool mouseOnSide(BoardSided board, bool extended=false) const;

        CellPosition getCellPosition(Board) const;
        SidePosition getSidePosition(BoardSided) const;

        void setMessage(std::string);
};