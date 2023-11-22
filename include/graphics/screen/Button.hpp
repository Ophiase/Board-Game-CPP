#pragma once

#include "Drawable.hpp"
#include "Handler.hpp"

class Button : public Drawable, public Handler {
    protected:
        sf::Vector2f position;
        sf::Vector2f size;
        
        Button(sf::RenderWindow & window) : Drawable{window} {};
    public:
        Button(sf::RenderWindow & window, sf::Vector2f position, sf::Vector2f size) : 
            Drawable{window}, Handler{}, 
            position{position}, size{size} 
            {};

        void handleEvent(sf::Event event);

        virtual void onClick(sf::Event event) { (void)event; };
        virtual void onHoover(sf::Event event) { (void)event; };

        sf::Vector2f getRelativeMousePosition() const;
};