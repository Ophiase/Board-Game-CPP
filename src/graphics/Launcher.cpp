#include "graphics/Launcher.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "utils/Cli.hpp"
#include "geometry/Geometry.hpp"
#include "graphics/Screen.hpp"
#include "graphics/Menu.hpp"

Launcher::Launcher() : window {
        sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y), DEFAULT_WINDOW_TITLE, DEFAULT_STYLE,
} {
    //this->window.setVerticalSyncEnabled(true);
    this->window.setFramerateLimit(60);
    this->window.setVisible(false);

    ResourcesLoader::initialize();
    if (!ResourcesLoader::load()) {
        std::cerr << "Error loading textures!" << std::endl;
        exit(1);
    };
    
    Cli::info("Start Launcher");
}

void Launcher::updateView(sf::RectangleShape & background, sf::RectangleShape & foreground) {
    float backgroundScale = 1.2;
    
    sf::FloatRect visibleArea = Geometry::fit(
        Geometry::toFloat(this->window.getSize()), 
        foreground.getPosition(),
        foreground.getSize()
    );

    auto backgroundTransform = Geometry::scaleRect(
        Geometry::fit(background.getSize(), visibleArea),
        backgroundScale
    );

    Geometry::applyFloatRectToRectangleShape(
        background, backgroundTransform
    );

    this->window.setView(sf::View(visibleArea));
}

void Launcher::run() {
    this->window.setVisible(true);
    Menu menu{};
    Screen *focus = &menu;
    
    float backgroundRatio = Geometry::toRatio(Texture::BackgroundMainBlur);
    sf::RectangleShape background(sf::Vector2f(1.0,backgroundRatio));
    background.setTexture(&ResourcesLoader::getTexture(Texture::BackgroundMainBlur));
    
    float foregroundRatio = Geometry::toRatio(Texture::BackgroundMenu2);
    sf::RectangleShape foreground(sf::Vector2f(1.0, 1/foregroundRatio));
    foreground.setTexture(&ResourcesLoader::getTexture(Texture::BackgroundMenu2));
    foreground.setPosition(0.0, (1-1/foregroundRatio)/2);
    foreground.setFillColor(sf::Color::White);
 
    updateView(background, foreground);

    while (this->window.isOpen()) {
        if (!focus->isAlive() && (focus != &menu)) {
            delete focus;
            focus = &menu;
        };

        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();

            if (event.type == sf::Event::Resized) {
                /*
                    This code section was meant to impose limits
                    on window size and aspect ratio.

                    A bug on linux makes it impossible : 
                    https://github.com/SFML/SFML/issues/2124
                    
                    sf::Vector2u size{
                        std::max(this->window.getSize().x, MINIMUM_WINDOW_SIZE.x),
                        std::max(this->window.getSize().y, MINIMUM_WINDOW_SIZE.y),
                    };
                    this->window.setSize(size);
                */

                updateView(background, foreground);
            }

            Screen *sucessor = focus->handleEvent(event);
            focus = (sucessor == nullptr) ? focus : sucessor;
        }

        this->window.clear(sf::Color::Black);

        this->window.draw(background);
        this->window.draw(foreground);
        
        focus->draw(window);
        
        this->window.display();
    }
}