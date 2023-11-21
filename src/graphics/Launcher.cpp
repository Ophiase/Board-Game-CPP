#include "graphics/Launcher.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "geometry/Geometry.hpp"

Launcher::Launcher() : window {
        sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y), DEFAULT_WINDOW_TITLE, DEFAULT_STYLE,
} {
    //this->window.setVerticalSyncEnabled(true);
    this->window.setFramerateLimit(60);
    this->window.setVisible(false);

    std::cout << "Launcher !" << std::endl;
    
    ResourcesLoader::initialize();
    if (!ResourcesLoader::load()) {
        std::cerr << "Error loading textures!" << std::endl;
        exit(1);
    };
}

void Launcher::run() {
    this->window.setVisible(true);
    

    auto background_ratio = Geometry::toRatio(Texture::BackgroundMainBlur);
    sf::RectangleShape background(sf::Vector2f(1.0,background_ratio));
    background.setTexture(&ResourcesLoader::getTexture(Texture::BackgroundMainBlur));
    background.setPosition(0.0, (1-background_ratio)/2);

    float ratio = (float)DEFAULT_WINDOW_SIZE.y/(float)DEFAULT_WINDOW_SIZE.x;
    sf::RectangleShape foreground(sf::Vector2f(1.0, ratio));
    foreground.setTexture(&ResourcesLoader::getTexture(Texture::BackgroundMainBlur));
    foreground.setPosition(0.0, (1-ratio)/2);
    foreground.setFillColor(sf::Color::White);

    while (this->window.isOpen()) {
        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();

            if (event.type == sf::Event::Resized) {
                /*
                    bug linux : https://github.com/SFML/SFML/issues/2124
                    
                    sf::Vector2u size{
                        std::max(this->window.getSize().x, MINIMUM_WINDOW_SIZE.x),
                        std::max(this->window.getSize().y, MINIMUM_WINDOW_SIZE.y),
                    };
                    this->window.setSize(size);
                */

                sf::FloatRect visibleArea(0, 0, 1.0, 1.0);
                this->window.setView(sf::View(visibleArea));
            }
        }

        this->window.clear(sf::Color::Black);
        this->window.draw(background);
        this->window.draw(foreground);
        this->window.display();
    }
}