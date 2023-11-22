#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Handler {
    public:
        virtual void handleEvent(sf::Event event) = 0;
};