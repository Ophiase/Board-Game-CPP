#include "graphics/screen/Container.hpp"
#include "graphics/screen/Canvas.hpp"
#include "geometry/Geometry.hpp"
#include "algorithm"
#include "utils/Cli.hpp"

#include <sstream>
#include <iostream>
template <typename T>
std::string pointerToString(T* ptr) {
    // Utiliser un flux de chaîne pour convertir le pointeur en une chaîne
    std::stringstream stream;
    stream << ptr;

    // Retourner la chaîne résultante
    return stream.str();
}

Container::Container(Container *parent, bool withHandler) : DrawableHandler{parent->launcher} {
    parent->addDrawing(this);
    if (withHandler)
        parent->addHandler(this);
}

Container::~Container() {
    for (Canvas *d : deleteList)
        delete d;
}

void Container::addObjectToDelete(Canvas *d) {
    this->deleteList.push_back(d);
};

bool Container::handleEvent(sf::Event event) {
    if (!enabled) return false;
    
    for (auto handler : this->handlers)
        if (std::get<0>(handler)->handleEvent(event))
            return true;

    return false;
}

void Container::draw() {
    if (!enabled) return;

    for (auto drawable : this->drawables)
        std::get<0>(drawable)->draw();
}

void Container::addHandler(Handler *handler, Priority priority) {
    this->handlers.push_back(std::make_tuple(handler, priority));
    sortByPriorityDecrementing<Handler>(handlers);
}

void Container::addDrawing(Drawable *drawable, Priority priority) {
    this->drawables.push_back(std::make_tuple(drawable, priority));
    sortByPriorityDecrementing<Drawable>(drawables);
}

void Container::add(DrawableHandler *drawableHandler, Priority priority) {
    addDrawing(drawableHandler, priority);
    addHandler(drawableHandler, priority);
}

sf::Vector2f Container::mouseWorldSpace() const {
    return Geometry::screenPositionToWorldSpace(
        sf::Mouse::getPosition(this->getConstRenderWindow()),
        this->getConstRenderWindow().getSize(),
        this->getScreen()
        );
};

sf::Vector2f Container::getRelativeMousePosition() const {
    return Geometry::toFloat(sf::Mouse::getPosition(this->getConstRenderWindow()));
}

void Container::setEnabled(bool enabled) {
    this->enabled = enabled;
}

bool Container::isEnabled() const {
    return this->enabled;
}