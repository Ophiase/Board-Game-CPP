#pragma once

#include "graphics/screen/Screen.hpp"
#include "SFML/Graphics.hpp"
#include "utils/GarbageManager.hpp"
#include "graphics/screen/Text.hpp"

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

        void setContext(Container *context);
        Container *getContext(MenuContext context);
        std::vector<Container*> getAllContext();

        void gameSelectionInit();
        void lootOptionInit();
        void checkersOptionInit();
        void bullTrickerOptionInit();

        // ---------------------------------
        // Loot Options

            int lootNPlayers{2};
            Text lootNplayersText{&lootOption, "2"};
            int lootNBots{0};
            Text lootnBotsText{&lootOption, "0"};
            void lootEditNPlayers(int);
            void lootEditNBots(int); 
        
        // ---------------------------------

    public:
        Menu(Launcher *launcher);
        Screen *successor{nullptr};

        void draw();
        void setContext(MenuContext context);      
};