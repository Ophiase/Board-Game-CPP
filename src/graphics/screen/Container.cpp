#include "graphics/screen/Container.hpp"

#include "algorithm"


void Container::handleEvent(sf::Event event) {
    for (auto handler : this->handlers)
        std::get<0>(handler).handleEvent(event);
}

void Container::draw(sf::RenderWindow& window) {
    for (auto drawable : this->drawables)
        std::get<0>(drawable).draw(window);
}

void Container::addHandle(Handler handle, Priority priority) {
    this->handlers.push_back(std::make_tuple(handle, priority));
    sortByPriorityDecrementing(handlers);
}

void Container::addDrawing(Drawable drawable, Priority priority) {
    this->drawables.push_back(std::make_tuple(drawable, priority));
    sortByPriorityDecrementing(drawables);
}