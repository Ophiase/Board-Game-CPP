#include "graphics/ResourcesLoader.hpp"

#include "utils/Cli.hpp"

#include <tuple>
#include <iostream>


std::map<Texture::SourceTexture, std::string> ResourcesLoader::texturePathMap;
std::map<Texture::SourceTexture, sf::Texture> ResourcesLoader::textureMap;


void ResourcesLoader::initialize() {
    using namespace Texture;
    using namespace std;

    std::tuple<SourceTexture, std::string> associations[] = {
        make_tuple(BackgroundMain, "background/main"),
        make_tuple(BackgroundMainBlur, "background/main_blur"),
        make_tuple(BackgroundMenu2, "background/menu_2"),
        make_tuple(BackgroundMenu, "background/menu"),
        make_tuple(BackgroundPanelBot, "background/panel_bot"),
        make_tuple(BackgroundPanelLeft, "background/panel_left"),
        make_tuple(BackgroundPanelRight, "background/panel_right"),
        make_tuple(BackgroundPanelTop, "background/panel_top"),
        
        make_tuple(TileBrick, "tile/brick"),
        make_tuple(TileDesert, "tile/desert"),
        make_tuple(TileRock, "tile/rock"),
        make_tuple(TileSheep, "tile/sheep"),
        make_tuple(TileWheat, "tile/wheat"),
        make_tuple(TileWood, "tile/wood")
    };

    for (const auto& association : associations) {
        SourceTexture texture;
        std::string texturePath;
        std::tie(texture, texturePath) = association;

        std::string fullPath = 
            "resources/images/" + std::string(texturePath) + ".png";
        texturePathMap[texture] = fullPath;
    }}

bool ResourcesLoader::load() {
    Cli::info("Loading Resources ...");

    for (const auto& pair : texturePathMap) {
        sf::Texture texture;
        if (!texture.loadFromFile(pair.second))
            return false;
        textureMap[pair.first] = texture;
    }


    return true;
}

const sf::Texture& ResourcesLoader::getTexture(Texture::SourceTexture texture) {
    return textureMap.at(texture);
}
