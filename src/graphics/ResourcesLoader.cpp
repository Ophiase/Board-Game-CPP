#include "graphics/ResourcesLoader.hpp"

#include "utils/Cli.hpp"

#include <tuple>
#include <iostream>


std::map<Texture::SourceTexture, std::string> ResourcesLoader::texturePathMap;
std::map<Texture::SourceTexture, sf::Texture> ResourcesLoader::textureMap;

std::map<Font::SourceFont, std::string> ResourcesLoader::fontPathMap;
std::map<Font::SourceFont, sf::Font> ResourcesLoader::fontMap;

std::map<Shader::SourceShader, std::string> ResourcesLoader::shaderPathMap;
std::map<Shader::SourceShader, sf::Shader> ResourcesLoader::shaderMap;

void ResourcesLoader::initializeTextures() {
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
    }
}

void ResourcesLoader::initializeFonts() {
    using namespace Font;
    using namespace std;

    std::tuple<SourceFont, std::string> associations[] = {
        make_tuple(OpenSansBold, "OpenSans-Bold"),
        make_tuple(OpenSansExtraBold, "OpenSans-ExtraBold"),
        make_tuple(OpenSansRegular, "OpenSans-Regular"),
        make_tuple(OpenSansLight, "OpenSans-Light"),
        make_tuple(OpenSansSemiBold, "OpenSans-Semibold")
    };

    for (const auto& association : associations) {
        SourceFont font;
        std::string fontPath;
        std::tie(font, fontPath) = association;

        std::string fullPath = 
            "resources/fonts/" + std::string(fontPath) + ".ttf";
        fontPathMap[font] = fullPath;
    }
}

void ResourcesLoader::initializeShaders() {
    using namespace Shader;
    using namespace std;

    std::tuple<SourceShader, std::string> associations[] = {
        make_tuple(Mask, "mask")
    };

    for (const auto& association : associations) {
        SourceShader shader;
        std::string shaderPath;
        std::tie(shader, shaderPath) = association;

        std::string fullPath = 
            "resources/shaders/" + std::string(shaderPath) + ".frag";
        shaderPathMap[shader] = fullPath;
    }
}

void ResourcesLoader::initialize() {
    initializeTextures();
    initializeFonts();
    initializeShaders();
}

bool ResourcesLoader::loadTextures() {
    for (const auto& pair : texturePathMap)
        if (!textureMap[pair.first].loadFromFile(pair.second))
            return false;
    return true;    
}

bool ResourcesLoader::loadFonts() {
    for (const auto& pair : fontPathMap)
        if (!fontMap[pair.first].loadFromFile(pair.second))
            return false;
    return true;    
}

bool ResourcesLoader::loadShaders() {
    for (const auto& pair : shaderPathMap)
        if (!shaderMap[pair.first].loadFromFile(pair.second, sf::Shader::Fragment))
            return false;
    return true;    
}

bool ResourcesLoader::load() {
    Cli::info("Loading Resources ...");
    return loadTextures() && loadFonts() && loadShaders();
}

const sf::Texture& ResourcesLoader::getTexture(Texture::SourceTexture texture) {
    return textureMap.at(texture);
}

const sf::Font& ResourcesLoader::getFont(Font::SourceFont font) {
    return fontMap.at(font);
}

sf::Shader *ResourcesLoader::getShader(Shader::SourceShader shader) {
    return &shaderMap.at(shader);
}
