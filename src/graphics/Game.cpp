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
    this->score.fit(sf::FloatRect{0.67, 0.33, 0.25, 0.08});
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
void Game::playAction() { throw NotImplemented(); };
void Game::AIinit() { throw NotImplemented(); };
void Game::AIturn() { throw NotImplemented(); };

// ---------------------------------------------


void Game::draw() {
    Screen::draw();  
    this->getRenderWindow().draw(this->checkerBoardImage);
};

void Game::updateBoard(Board board) {
    this->checkBoardTexture.clear(sf::Color::Transparent);

    /* baked 8x8 board (faster)
        sf::RectangleShape background{
            Geometry::toFloat(checkBoardTexture.getSize())};
        background.setTexture(ResourcesLoader::getTexture(Texture::CheckerBoard0));
        this->checkBoardTexture.draw(background);
    */

    const sf::Vector2f tileGeometry = Geometry::toFloat(
        sf::Vector2i{
            (int)checkBoardTexture.getSize().x / (int)board.getDimension(),
            (int)checkBoardTexture.getSize().y / (int)board.getDimension()
        }
    );
    
    const sf::Vector2f tileOrigin = sf::Vector2f{
        tileGeometry.x/2, 
        tileGeometry.y/2
    };

    sf::RectangleShape 
        evenTile{tileGeometry}, 
        oddTile{tileGeometry};

    evenTile.setTexture(ResourcesLoader::getTexture(Texture::TileDesert));
    evenTile.setOrigin(tileOrigin);
    evenTile.setScale(sf::Vector2f{CELL_TRESHOLD, CELL_TRESHOLD});
    
    oddTile.setTexture(ResourcesLoader::getTexture(Texture::TileWheatBis));
    oddTile.setOrigin(tileOrigin);
    oddTile.setScale(sf::Vector2f{CELL_TRESHOLD, CELL_TRESHOLD});

    for (uint y = 0; y < board.getDimension(); y++)
    for (uint x = 0; x < board.getDimension(); x++) {
        sf::RectangleShape *tile = board.isCellEven(x, y) ? 
            &evenTile : &oddTile;

        tile->setPosition(
            (tileGeometry.x/2) + (tileGeometry.x * (int)x),
            (tileGeometry.y/2) + (tileGeometry.y * (int)y)
        );

        this->checkBoardTexture.draw(*tile);
    }

    this->checkBoardTexture.display();
};

void Game::updateBoardContent(Board board) {
    this->updateBoard(board);

    const int BOARD_DIMENSION = board.getDimension();

    const float cellSpace = (float)(this->checkBoardTexture.getSize().x / BOARD_DIMENSION); 
    const float pieceScale = CELL_TRESHOLD;
    const float pieceSize = cellSpace*pieceScale;

    sf::RectangleShape piece{sf::Vector2f{
        pieceSize, pieceSize
    }};

    float offset = pieceSize / 2.0;
    for (int x = 0; x < BOARD_DIMENSION; x++) for (int y = 0; y < BOARD_DIMENSION; y++) {
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

void Game::updateBoardSidedContent(BoardSided board) {
    this->updateBoardContent(board);
    
    const int BOARD_DIMENSION = board.getDimension();

    const float checkerBoardSize = (float)(this->checkBoardTexture.getSize().x); 
    const float sideSpace = (float)(checkerBoardSize / BOARD_DIMENSION); 

    const float pieceScale = 0.7;
    const float pieceHeight = sideSpace*pieceScale;
    const float pieceWidth = pieceHeight*0.3f;
    const float cx = pieceWidth / 2.0;
    const float cy = pieceHeight / 2.0;

    sf::RectangleShape piece{sf::Vector2f{
        pieceWidth, pieceHeight
    }};

    const sf::Vector2f origin{cx, cy};
    piece.setOrigin(origin);

    const float ratio = ((checkerBoardSize - pieceWidth)/BOARD_DIMENSION);
    
    // vertical
    for (int x = 0; x < BOARD_DIMENSION+1; x++)
    for (int y = 0; y < BOARD_DIMENSION; y++) {
        SidePiece cell = board.getCell(SidePosition{SideVector{x, y}, false});

        if (!cell.isNone()) {
            const sf::Vector2f position{
                ratio*x + cx,
                (sideSpace * y) + (sideSpace/2)
            };

            piece.setPosition(position);
            piece.setTexture(ResourcesLoader::getTexture(cell));
            
            checkBoardTexture.draw(piece);
        }
    }

    // horizontal
    piece.rotate(90);

    for (int x = 0; x < BOARD_DIMENSION; x++)
    for (int y = 0; y < BOARD_DIMENSION+1; y++) {
        SidePiece cell = board.getCell(SidePosition{SideVector{x, y}, true});

        if (!cell.isNone()) {
            const sf::Vector2f position{
                (sideSpace * x) + (sideSpace/2),
                ratio*y + cx
            };

            piece.setPosition(position);
            piece.setTexture(ResourcesLoader::getTexture(cell));
            
            checkBoardTexture.draw(piece);
        }
    }


    // done
    this->checkBoardTexture.display();
};

// ---------------------------------------------

sf::Vector2f Game::insideCellPosition(Board board) const {
    sf::Vector2f mousePosition = this->mouseBoardPosition();
    return sf::Vector2f{
        fmodf(mousePosition.x * board.getDimension(), 1.0) - 0.5f,
        fmodf(mousePosition.y * board.getDimension(), 1.0) - 0.5f
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

bool Game::mouseOnCase(Board board, bool extended) const {
    if (!extended && !mouseInsideCheckerBoard())
        return false;

    sf::Vector2f positionInsideCell = this->insideCellPosition(board);
    
    return 
        (abs(positionInsideCell.x) < (CELL_TRESHOLD / 2.0)) &&
        (abs(positionInsideCell.y) < (CELL_TRESHOLD / 2.0));
};

bool Game::mouseOnSide(BoardSided board, bool extended) const {
    if (!extended && !mouseInsideCheckerBoard())
        return false;
    
    return !mouseOnCase(board, true);
};

CellPosition Game::getCellPosition(Board board) const {
    sf::Vector2f mousePosition = this->mouseBoardPosition();
    return CellPosition{
        (int)(std::floor(mousePosition.x * board.getDimension())),
        (int)(std::floor(mousePosition.y * board.getDimension()))
    };
};

SidePosition Game::getSidePosition(BoardSided board) const {
    auto cellPosition = getCellPosition(board);
    sf::Vector2f positionInsideCell = this->insideCellPosition(board);

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