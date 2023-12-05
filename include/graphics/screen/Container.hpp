#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "DrawableHandler.hpp"
#include "Container.hpp"

using Priority = int;

class Canvas;

/*
    Can contains sub component
*/
class Container : public DrawableHandler {
private:
    template <typename T>
    static bool compareByPriority(
        const std::tuple<T*, Priority>& a, 
        const std::tuple<T*, Priority>& b
        ) {
            
        return std::get<1>(a) > std::get<1>(b);
    }

    template <typename T>
    void sortByPriorityDecrementing(
        std::vector<std::tuple<T*, Priority>>& items
        ) {
        std::sort(items.begin(), items.end(), compareByPriority<T>);
    }

    std::vector<std::tuple<Handler*, Priority>> handlers;
    std::vector<std::tuple<Drawable*, Priority>> drawables;
    std::vector<Canvas*> deleteList;
public:
    Container(Launcher *launcher) : DrawableHandler{launcher} {};
    Container(Container *parent, bool withHandler=true);
    ~Container();

    void addObjectToDelete(Canvas *d);

    void handleEvent(sf::Event event);
    void draw();

    void addHandler(Handler *handle, Priority priority = 0);
    void addDrawing(Drawable *drawable, Priority priority = 0);
    void add(DrawableHandler *drawableHandler, Priority priority = 0);

    sf::Vector2f mouseWorldSpace() const;
    virtual sf::Vector2f getRelativeMousePosition() const;
};