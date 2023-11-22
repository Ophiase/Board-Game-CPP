#pragma once

#include "graphics/screen/Screen.hpp"

class Game : Screen {
    Game(sf::RenderWindow & window) : Screen{window} {};
};