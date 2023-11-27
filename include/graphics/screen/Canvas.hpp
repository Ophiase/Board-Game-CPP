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
        Canvas(Launcher *launcher) = delete;
        
        Canvas(Container *parent,
            sf::Vector2f position, sf::Vector2f size = sf::Vector2f{1.0, 1.0},
            bool withHandler = true
            );

        sf::Vector2f getSize() const;
        void setSize(sf::Vector2f size);
        
        /*
            Set x size and keep x/y ratio.
        */
        void setSizeX(float sx);

        /*
            Set y size and keep x/y ratio.
        */
        void setSizeY(float sy);
        
        sf::Vector2f getPosition() const;
        void setPosition(sf::Vector2f position);

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