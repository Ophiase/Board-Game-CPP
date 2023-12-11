#pragma once

#include "graphics/screen/Screen.hpp"
#include "SFML/Graphics.hpp"
#include "utils/GarbageManager.hpp"

enum class MenuContext {
    GameSelection,
    LootOption,
    CheckersOption,
    BullTrickerOption
};

class Menu : public Screen {
    private:
        Container gameSelection;
        Container lootOption;
        Container checkersOption;
        Container bullTrickerOption;

        Container *getContext(MenuContext canvas);
        std::vector<Container*> getAllContext();

        void gameSelectionInit();
        void lootOptionInit();
        void checkersOptionInit();
        void bullTrickerOptionInit();

        void setContext(Container *context);
    public:
        Menu(Launcher *launcher);
        Screen *successor{nullptr};

        void draw();
        void setContext(MenuContext context);
};