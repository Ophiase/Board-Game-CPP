#pragma once

#include "Drawable.hpp"
#include "Handler.hpp"

class DrawableHandler : public Drawable, public Handler {
    public:
        DrawableHandler(Launcher *launcher) : Drawable{launcher}, Handler() {};

};