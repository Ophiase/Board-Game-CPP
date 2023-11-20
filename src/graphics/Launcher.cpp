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

    //resourceLoader.load()
}

void Launcher::run() {
    this->window.setVisible(true);
    
    float ratio = (float)DEFAULT_WINDOW_SIZE.y/(float)DEFAULT_WINDOW_SIZE.x;
    sf::RectangleShape background(
        sf::Vector2f(1.0, ratio)
    );
    background.setFillColor(sf::Color::White);
    background.setPosition(0.0, (1-ratio)/2);

    while (this->window.isOpen()) {
        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, 1.0, 1.0);
                this->window.setView(sf::View(visibleArea));
            }
        }

        this->window.clear(sf::Color::Black);
        this->window.draw(background);
        this->window.display();
    }
}