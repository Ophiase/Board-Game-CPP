#pragma once

#include <SFML/Graphics.hpp>

class Drawable {
    public:
        sf::RenderWindow & window;
        Drawable(sf::RenderWindow& window) : window{window} {};
        Drawable(const Drawable & drawable) : Drawable{drawable.window} {};
        Drawable& operator=(const Drawable& other) {
            // should i copy the other.window ?
            (void) other;
            return *this; 
        };

        virtual void draw() {};
};