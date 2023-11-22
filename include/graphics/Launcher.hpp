#pragma once

#include "Game.hpp"
#include "ResourcesLoader.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include "screen/Screen.hpp"

/*
    This class provides a graphical interface to launch a Game.
*/
class Launcher {
    private:
        const sf::Vector2u DEFAULT_WINDOW_SIZE{1024, 800};
        const std::string DEFAULT_WINDOW_TITLE = "Board Game";
        const sf::Uint32 DEFAULT_STYLE = sf::Style::Default;

        const sf::Vector2u MINIMUM_WINDOW_SIZE{640, 360};
        const sf::Vector2f RATIO_RANGE{0.5, 2.0};

        sf::RenderWindow window;
        Screen *focus;

        sf::RectangleShape screen, maxRenderZone;
        sf::RectangleShape title, background, foreground;
        sf::Text titleText;

        void initView();
        void updateView();
        void adjustSize();
        void applyLimitMask();
    public:
        Launcher();
        void run();
};