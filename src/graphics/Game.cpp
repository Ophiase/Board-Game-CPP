#include "graphics/Game.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "utils/Cli.hpp"
#include "graphics/screen/Text.hpp"

Game::Game(Launcher *launcher, std::string title) : 
    Screen{launcher, title}, 
    currentPlayer{this, "Player : ?"} {

    Cli::info("Choose : " + title);

    this->checkerBoardImage.setTexture(
        ResourcesLoader::getTexture(Texture::CheckerBoard0));
    this->checkerBoardImage.setPosition(BOARD_POSITION);
    this->checkerBoardImage.setSize(BOARD_SIZE);

    this->currentPlayer.setSizeY(0.05);
    this->currentPlayer.center(sf::Vector2f{0.8, 0.05});
};

void Game::draw() {
    Screen::draw();
    this->getRenderWindow().draw(this->checkerBoardImage);
}