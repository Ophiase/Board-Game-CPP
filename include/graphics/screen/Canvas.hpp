#pragma once

#include "Container.hpp"

/*
    Container with a position and size.
*/
class Canvas : public Container {
    protected:
        sf::Vector2f position;
        sf::Vector2f size;
    public:
        Canvas(Launcher *launcher, 
            sf::Vector2f position, sf::Vector2f size
            ) = delete;
        
        Canvas(Container *parent,
            sf::Vector2f position, sf::Vector2f size,
            bool withHandler = true
            );

        void setSize(sf::Vector2f size);
        sf::Vector2f getSize() const;
        
        void setPosition(sf::Vector2f position);
        sf::Vector2f getPosition() const;

        /*
            size / 2.0
        */
        sf::Vector2f getRelMid() const; 

        /*
            position + size/2.0 
        */
        sf::Vector2f getMid() const;

        /*
            Right bot corner position
        */
        sf::Vector2f getEndCorner() const;

        /*
            canvas.position <- position - canvas.mid
        */
        void center(sf::Vector2f position);

        /*
            canvas.position <- position - relAnchor
        */
        void center(sf::Vector2f position, sf::Vector2f relAnchor);

        void setFloatRect(sf::FloatRect rect);
        sf::FloatRect getFloatRect() const;

        void fit(sf::FloatRect targetBbox);

        sf::Vector2f getRelativeMousePosition() const;
};