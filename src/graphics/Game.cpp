#include "graphics/Game.hpp"
#include "graphics/ResourcesLoader.hpp"
#include "utils/Cli.hpp"

Game::Game(Launcher *launcher, std::string title) : 
    Screen{launcher, title} {

    this->checkerBoardImage.setTexture(
        ResourcesLoader::getTexture(Texture::CheckerBoard0));
    this->checkerBoardImage.setPosition(BOARD_POSITION);
    this->checkerBoardImage.setSize(BOARD_SIZE);
};

void Game::draw() {
    Screen::draw();
    this->getRenderWindow().draw(this->checkerBoardImage);
}