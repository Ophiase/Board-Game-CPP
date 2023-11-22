#include "graphics/screen/Container.hpp"
#include "geometry/Geometry.hpp"
#include "algorithm"

void Container::handleEvent(sf::Event event) {
    for (auto handler : this->handlers)
        std::get<0>(handler).handleEvent(event);
}

void Container::draw() {
    for (auto drawable : this->drawables)
        std::get<0>(drawable).draw();
}

void Container::addHandler(Handler handle, Priority priority) {
    this->handlers.push_back(std::make_tuple(handle, priority));
    sortByPriorityDecrementing<Handler>(handlers);
}

void Container::addDrawing(Drawable drawable, Priority priority) {
    this->drawables.push_back(std::make_tuple(drawable, priority));
    sortByPriorityDecrementing<Drawable>(drawables);
}

sf::Vector2f Container::getRelativeMousePosition() const {
    return Geometry::toFloat(sf::Mouse::getPosition(this->window));
}