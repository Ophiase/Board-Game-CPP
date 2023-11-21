#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

namespace Texture {
    /*
        Linked to resources into ResourcesLoader::initialize.
    */
    enum SourceTexture {
        BackgroundMain,
        BackgroundMainBlur,
        BackgroundMenu2,
        BackgroundMenu,
        BackgroundPanelBot,
        BackgroundPanelLeft,
        BackgroundPanelRight,
        BackgroundPanelTop,

        TileBrick,
        TileDesert,
        TileRock,
        TileSheep,
        TileWheat,
        TileWood
    };
}

class ResourcesLoader {
private:
    static std::map<Texture::SourceTexture, std::string> texturePathMap;
    static std::map<Texture::SourceTexture, sf::Texture> textureMap;

public:
    static void initialize();
    static bool load();
    static const sf::Texture& getTexture(Texture::SourceTexture texture);
};
