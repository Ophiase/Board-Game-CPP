#pragma once

#include <functional>
#include "Canvas.hpp"

class Button : virtual public Canvas {
    protected:
        const std::function<void(sf::Event)> onHoverFunction;
        const std::function<void(sf::Event)> onClickFunction;
    public:
        Button(Container *parent,
        std::function<void(sf::Event)> onHoverAction = [](sf::Event) {}, 
        std::function<void(sf::Event)> onClickAction = [](sf::Event) {},
        sf::Vector2f position = sf::Vector2f{0.0, 0.0}, 
        sf::Vector2f size = sf::Vector2f{1.0, 1.0},
        bool withHandler = true
        ) : Canvas{parent, position, size, withHandler},
        onHoverFunction(onHoverAction),
        onClickFunction(onClickAction) {};

        bool handleEvent(sf::Event event);
};