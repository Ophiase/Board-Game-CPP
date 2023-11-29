#include "graphics/Game.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "utils/Cli.hpp"
#include "graphics/screen/Text.hpp"
#include "geometry/Geometry.hpp"

Game::Game(Launcher *launcher, std::string title) : 
    Screen{launcher, title}, 
    currentPlayer{this, "Turn: ?"},
    exitButton{this, "Exit",
        [this](sf::Event) -> void{
            this->alive = false;
        }
    } {

    Cli::info("Choose : " + title);

    this->checkerBoardImage.setTexture(
        ResourcesLoader::getTexture(Texture::CheckerBoard0));
    this->checkerBoardImage.setPosition(BOARD_POSITION);
    this->checkerBoardImage.setSize(BOARD_SIZE);

    this->currentPlayer.setSizeY(0.04);
    this->currentPlayer.center(sf::Vector2f{0.75, 0.05});

    this->exitButton.setSizeY(0.04);
    this->exitButton.center(sf::Vector2f{0.10, 0.05});
};

void Game::setCurrentPlayer(std::string currentPlayer) {
    this->currentPlayer.setText("Turn: " + currentPlayer);
};

void Game::draw() {
    Screen::draw();
    
    sf::Vector2f v{
        -0.004,0.004
    };
    this->checkerBoardImage.move(v);
    this->getRenderWindow().draw(this->checkerBoardImage);
    this->checkerBoardImage.move(-v);

    this->getRenderWindow().draw(this->checkerBoardImage);
}