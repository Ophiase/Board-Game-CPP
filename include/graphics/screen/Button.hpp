#pragma once

#include "Container.hpp"

class Button : public DrawableHandler {
    protected:
        sf::Vector2f position;
        sf::Vector2f size;
        
        Button(Container *container) : DrawableHandler{container->getLauncher()} {};
    public:
        Button(Container *container, sf::Vector2f position, sf::Vector2f size);

        void handleEvent(sf::Event event);

        virtual void onClick(sf::Event event) { (void)event; };
        virtual void onHoover(sf::Event event) { (void)event; };

        sf::Vector2f getRelativeMousePosition() const;
};