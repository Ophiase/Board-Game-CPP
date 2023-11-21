#pragma once

#include "Game.hpp"
#include "ResourcesLoader.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

/*
    This class provides a graphical interface to launch a Game.
*/
class Launcher {
    private:
        const sf::Vector2u DEFAULT_WINDOW_SIZE{1024, 800};
        const std::string DEFAULT_WINDOW_TITLE = "Board Game Launcher";
        const sf::Uint32 DEFAULT_STYLE = sf::Style::Default;

        const sf::Vector2u MINIMUM_WINDOW_SIZE{640, 360};
        const sf::Vector2f RATIO_RANGE{0.5, 2.0};

        sf::RenderWindow window;

        void updateView(sf::RectangleShape & background, sf::RectangleShape & foreground);
    public:
        Launcher();
        void run();
};