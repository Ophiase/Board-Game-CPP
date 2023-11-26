#pragma once

#include "Canvas.hpp"

class Button : public Canvas {
    protected:
    public:
        void handleEvent(sf::Event event);
        virtual void onClick(sf::Event event) { (void)event; };
        virtual void onHoover(sf::Event event) { (void)event; };
};