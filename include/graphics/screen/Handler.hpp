#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Handler {
    public:
        Handler() = delete;
        virtual void handleEvent(sf::Event event) { (void)event; };
};