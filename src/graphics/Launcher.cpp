#include "graphics/Launcher.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

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
    while (this->window.isOpen()) {
        loop();
        this->window.display();
    }
}

void Launcher::loop() {
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            this->window.close();

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            this->window.setView(sf::View(visibleArea));
        }
    }

    this->window.clear(sf::Color::Black);

    sf::RectangleShape background(sf::Vector2f(800, 600));
    background.setFillColor(sf::Color::White);
    background.setPosition(0, 0);

    this->window.draw(background);
}
