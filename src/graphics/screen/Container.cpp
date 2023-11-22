#pragma once

#include "graphics/screen/Container.hpp"

#include "algorithm"

void Container::handleEvent(sf::Event event) {
    for (auto handler : this.handlers)
        handler.handleEvent(event);
}

void Container::draw(sf::RenderWindow& window) {
    for (auto drawable : this.drawbles)
        drawable.draw(window);
}

void Container::addHandle(Handler handle, Priority priority = 0) {
    this.handlers.push_back(std::tuple{handle, priority});
    sortByPriorityDecrementing(handlers);
}

void Container::addDrawing(Drawable drawable, Priority priority = 0) {
    this.drawables.push_back(std::tuple{drawable, priority});
    sortByPriorityDecrementing(drawables);
}