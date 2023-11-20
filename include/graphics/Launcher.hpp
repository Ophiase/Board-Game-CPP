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
        const sf::Vector2u DEFAULT_WINDOW_SIZE{800, 600};
        const std::string DEFAULT_WINDOW_TITLE = "Board Game Launcher";
        const sf::Uint32 DEFAULT_STYLE = sf::Style::Default;

        sf::RenderWindow window;

        ResourcesLoader resourceLoader;

        void loop();
    public:
        Launcher();
        void run();
};