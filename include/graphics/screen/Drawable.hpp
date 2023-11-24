#pragma once

#include <SFML/Graphics.hpp>

class Launcher;

class Drawable {
    protected:
        Launcher *launcher;
    public:
        Drawable(Launcher *launcher) : launcher{launcher} {};
        Drawable(const Drawable & drawable) : Drawable{drawable.launcher} {};
        Drawable& operator=(const Drawable& other);

        virtual void draw() {};

        sf::FloatRect getRenderZone();
        sf::RenderWindow & getRenderWindow();
        sf::RenderWindow const & getConstRenderWindow() const;
        Launcher *getLauncher();
};