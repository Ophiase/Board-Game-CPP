#pragma once

#include <functional>
#include "Canvas.hpp"

class Button : virtual public Canvas {
    protected:
        const std::function<void(sf::Event)> onHoverFunction;
        const std::function<void(sf::Event)> onClickFunction;
    public:
        Button(Container *parent,
        sf::Vector2f position, 
        std::function<void(sf::Event)> onHoverAction, 
        std::function<void(sf::Event)> onClickAction,
        sf::Vector2f size = sf::Vector2f{1.0, 1.0},
        bool withHandler = true
        ) : Canvas{parent, position, size, withHandler},
        onHoverFunction(onHoverAction),
        onClickFunction(onClickAction) {};

        void handleEvent(sf::Event event);
};