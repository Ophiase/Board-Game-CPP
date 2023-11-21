#include "graphics/Menu.hpp"

std::string Menu::getTitle() { return "Menu"; }

void Menu::draw(sf::RenderWindow& window) {
    (void)window;
}

Screen *Menu::handleEvent(sf::Event event) {
    (void)event;

    return nullptr;
}