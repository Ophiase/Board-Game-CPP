#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Handler.hpp"
#include "Drawable.hpp"
#include "Container.hpp"

using Priority = int;

class Container : Drawable, Handler {
private:
    template <typename T>
    static bool compareByPriority(
        const std::tuple<T, Priority>& a, 
        const std::tuple<T, Priority>& b
        ) {
            
        return std::get<1>(a) > std::get<1>(b);
    }

    template <typename T>
    void sortByPriorityDecrementing(
        std::vector<std::tuple<T, Priority>>& items
        ) {
        
        std::sort(items.begin(), items.end(), compareByPriority<T>);
    }

    std::vector<std::tuple<Handler, Priority>> handlers;
    std::vector<std::tuple<Drawable, Priority>> drawables;
public:
    void handleEvent(sf::Event event);
    void draw(sf::RenderWindow& window);

    void addHandle(Handler handle, Priority priority = 0);
    void addDrawing(Drawable drawable, Priority priority = 0);
};

