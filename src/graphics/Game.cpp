#include "graphics/Game.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "utils/Cli.hpp"
#include "graphics/screen/Text.hpp"
#include "geometry/Geometry.hpp"
#include "utils/NotImplemented.hpp"
#include <cmath>

Game::Game(Launcher *launcher, std::string title, float CELL_TRESHOLD) : 
    Screen{launcher, title},
    currentPlayer{this, "Turn: ?"},
    score{this, ""},
    exitButton{this, "Exit",
        [this](sf::Event) -> void{
            this->alive = false;
        }, }, 
    message{this, "", sf::Vector2f{0.5,0.17}, 0.05},
    CELL_TRESHOLD{CELL_TRESHOLD} {

    Cli::info("Choose : " + title);

    setMessage("You can play!");

    auto boardRes = ResourcesLoader::getTexture(Texture::CheckerBoard0)->getSize();
    this->checkBoardTexture.create(boardRes.x, boardRes.y);
    this->checkerBoardImage.setTexture(&checkBoardTexture.getTexture());
    this->checkerBoardImage.setPosition(BOARD_POSITION);
    this->checkerBoardImage.setSize(BOARD_SIZE);

    this->currentPlayer.setSizeY(0.04);
    this->currentPlayer.center(sf::Vector2f{0.75, 0.05});

    this->exitButton.setSizeY(0.04);
    this->exitButton.center(sf::Vector2f{0.10, 0.05});

    {   
        auto scoreText = new Text{this, "Scores"};
        scoreText->setSizeY(0.05);
        scoreText->center(sf::Vector2f{0.8, 0.3});

        this->addObjectToDelete(scoreText);

        score.setSizeY(0.05);
        score.center(scoreText->getMid() + sf::Vector2f{0.0, 0.05});
    }

    {
        auto validationButton = new TextButton{
            this, "Play", [this](sf::Event) -> void {
                this->playAction();
            }
        }; 
        
        validationButton->setSizeY(0.05);
        validationButton->center(0.8, 0.65);
        this->addObjectToDelete(validationButton);
    }

    {
        auto cancelButton = new TextButton{
            this, "Cancel", [this](sf::Event) -> void {
                this->cancelAction();
            }
        }; 
        
        cancelButton->setSizeY(0.05);
        cancelButton->center(0.8, 0.45);
        this->addObjectToDelete(cancelButton);
    }
};

void Game::setCurrentPlayer(std::string currentPlayer) {
    this->currentPlayer.setText("Turn: " + currentPlayer);
};

void Game::setScores(std::vector<float> scores) {
    (void)scores;
    throw NotImplemented(); // need to crop digit
}

void Game::setScores(std::vector<int> scores) {
    std::string scoreText = "";

    for (uint i = 0; i < scores.size() - 1; i++)
        scoreText += (std::to_string(scores[i]) + " | ");
    scoreText += std::to_string(scores[scores.size()-1]);

    this->score.setText(scoreText, true);
}

/*
    <Warning> chars need to be supported by ResourcesLoader
*/
void Game::setMessage(std::string msg) {
    auto mid = this->message.getMid();
    this->message.setText(msg);
    this->message.center(mid);
};

// ---------------------------------------------

void Game::startTurn() { throw NotImplemented(); };
void Game::cancelAction() { throw NotImplemented(); };
void Game::playAction() { Cli::debug("no"); return; throw NotImplemented(); };
void Game::AIturn() { throw NotImplemented(); };

// ---------------------------------------------


void Game::draw() {
    Screen::draw();  
    this->getRenderWindow().draw(this->checkerBoardImage);
};

void Game::updateBoard() {
    this->checkBoardTexture.clear(sf::Color::Transparent);

    sf::RectangleShape background{
        Geometry::toFloat(checkBoardTexture.getSize())};
    background.setTexture(ResourcesLoader::getTexture(Texture::CheckerBoard0));
    this->checkBoardTexture.draw(background);

    this->checkBoardTexture.display();
};

void Game::updateBoardContent(Board board) {
    this->updateBoard();

    float cellSpace = (float)(this->checkBoardTexture.getSize().x / 8); 
    float pieceScale = 0.7;
    float pieceSize = cellSpace*pieceScale;

    sf::RectangleShape piece{sf::Vector2f{
        pieceSize, pieceSize
    }};

    float offset = pieceSize / 2.0;
    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
        CellPiece cell = board.getCell(CellPosition(x, y));
        if (!cell.isNone()) {
            float px = (cellSpace * (x + 0.5)) - offset;
            float py = (cellSpace * (y + 0.5)) - offset;

            piece.setPosition(px, py);
            piece.setTexture(ResourcesLoader::getTexture(cell));
            checkBoardTexture.draw(piece);
        }
    }

    this->checkBoardTexture.display();
};

void Game::updateBoardSidedContent(BoardSided boardSided) {
    this->updateBoardContent(boardSided);
    throw NotImplemented();  
};

// ---------------------------------------------

sf::Vector2f Game::insideCellPosition() const {
    sf::Vector2f mousePosition = this->mouseBoardPosition();
    return sf::Vector2f{
        fmodf(mousePosition.x * 8, 1.0) - 0.5f,
        fmodf(mousePosition.y * 8, 1.0) - 0.5f
    };
}

sf::Vector2f Game::mouseBoardPosition() const {
    auto mouse = this->mouseWorldSpace();
    return sf::Vector2f {
        (mouse.x - BOARD_POSITION.x)/BOARD_SIZE.x,
        (mouse.y - BOARD_POSITION.y)/BOARD_SIZE.y
    };
};

bool Game::mouseInsideCheckerBoard() const {
    return Geometry::insideBox(this->mouseWorldSpace(), 
        sf::FloatRect{BOARD_POSITION, BOARD_SIZE});
};

bool Game::mouseOnCase(bool extended) const {
    if (!extended && !mouseInsideCheckerBoard())
        return false;

    sf::Vector2f positionInsideCell = this->insideCellPosition();
    
    return 
        (abs(positionInsideCell.x) < (CELL_TRESHOLD / 2.0)) &&
        (abs(positionInsideCell.y) < (CELL_TRESHOLD / 2.0));
};

bool Game::mouseOnSide(bool extended) const {
    if (!extended && !mouseInsideCheckerBoard())
        return false;
    
    return !mouseOnCase(true);
};

CellPosition Game::getCellPosition() const {
    sf::Vector2f mousePosition = this->mouseBoardPosition();
    return CellPosition{
        (int)(std::floor(mousePosition.x * 8)),
        (int)(std::floor(mousePosition.y * 8))
    };
};

SidePosition Game::getSidePosition() const {
    auto cellPosition = getCellPosition();
    sf::Vector2f positionInsideCell = this->insideCellPosition();

    bool horizontal = 
        std::abs(positionInsideCell.x) < std::abs(positionInsideCell.y);

    if (horizontal)
        return SidePosition(
            SideVector {
                cellPosition.x,
                cellPosition.y + (int)(positionInsideCell.y > 0)
            }, true);
        
    return SidePosition(
            SideVector {
                cellPosition.x + (int)(positionInsideCell.x > 0),
                cellPosition.y
            }, false);
};