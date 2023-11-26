#include "graphics/ResourcesLoader.hpp"

#include "utils/Cli.hpp"

#include <tuple>
#include <iostream>

bool ResourcesLoader::_loaded{false};

std::map<Texture::SourceTexture, std::string> ResourcesLoader::texturePathMap;
std::map<Texture::SourceTexture, sf::Texture*> ResourcesLoader::textureMap;

std::map<char, std::string> ResourcesLoader::charTexturePathMap;
std::map<char, sf::Texture*> ResourcesLoader::charTextureMap;

std::map<Font::SourceFont, std::string> ResourcesLoader::fontPathMap;
std::map<Font::SourceFont, sf::Font*> ResourcesLoader::fontMap;

std::map<Shader::SourceShader, std::string> ResourcesLoader::shaderPathMap;
std::map<Shader::SourceShader, sf::Shader*> ResourcesLoader::shaderMap;

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

void ResourcesLoader::initializeCharTextures() {
    using namespace std;
    
    std::vector<std::tuple<char, std::string>> associations;

    // digits
    for (char c = '0'; c <= '9'; c++)
        associations.push_back(make_tuple(c, "d" + c));

    // uppercase
    for (char c = 'A'; c < 'Z'; c++)
        associations.push_back(make_tuple(c, "u_" + std::tolower(c)));

    // lowercase
    for (char c = 'a'; c < 'z'; c++)
        associations.push_back(make_tuple(c, "l_" + c));

    // special
    {
        vector<vector<int>> intervals{
            {32, 36},
            {40, 41},
            {43},
            {45, 46},
            {58},
            {60, 63},
            {91}, {93}, {95},
            {124}, {126}
        };

        for (vector<int> whichInterval : intervals) {
            if (whichInterval.size() == 1)
            whichInterval.push_back(whichInterval[0]);

            for (int c = whichInterval[0]; c <= whichInterval[1]; c++)
                associations.push_back(make_tuple((char)c, "s_" + Cli::formatInt(c, 3)));
        }

    }

    for (const auto& association : associations) {
        char c;
        std::string texturePath;
        std::tie(c, texturePath) = association;

        std::string fullPath = 
            "resources/images/char/" + std::string(texturePath) + ".png";
        charTexturePathMap[c] = fullPath;
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
        make_tuple(Test, "test"),
        make_tuple(Mask, "mask"),
        make_tuple(Coord, "coord")
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
    if (_loaded) exit(1);
    
    initializeTextures();
    initializeFonts();
    initializeShaders();
    initializeCharTextures();
}

bool ResourcesLoader::loadTextures() {
    for (const auto& pair : texturePathMap) {
        auto texture = new sf::Texture{};
        if (!texture->loadFromFile(pair.second))
            return false;
        textureMap[pair.first] = texture;
    }
    return true;    
}

bool ResourcesLoader::loadCharTextures() {
    for (const auto& pair : charTexturePathMap) {
        auto charTexture = new sf::Texture{};
        if (!charTexture->loadFromFile(pair.second))
            return false;
        charTextureMap[pair.first] = charTexture;
    }
    return true;    
}

bool ResourcesLoader::loadFonts() {
    for (const auto& pair : fontPathMap) {
        auto font = new sf::Font{};
        if (!font->loadFromFile(pair.second))
            return false;
        fontMap[pair.first] = font;
    }
    return true;    
}

bool ResourcesLoader::loadShaders() {
    for (const auto& pair : shaderPathMap) {
        auto shader = new sf::Shader{};
        if (!shader->loadFromFile(pair.second, sf::Shader::Fragment))
            return false;
        shaderMap[pair.first] = shader;
    }
    return true;    
}

bool ResourcesLoader::load() {
    if (_loaded) exit(1);
    Cli::info("Loading Resources ...");
    _loaded = loadTextures() && loadFonts() && loadShaders() && loadCharTextures();
    return ResourcesLoader::_loaded;
}

bool ResourcesLoader::loaded() { 
    return _loaded; 
    }

sf::Texture *ResourcesLoader::getTexture(Texture::SourceTexture texture) {
    return textureMap.at(texture);
}

sf::Texture *ResourcesLoader::getCharTexture(char c) {
    return charTextureMap.at(c);
}

sf::Font *ResourcesLoader::getFont(Font::SourceFont font) {
    return fontMap.at(font);
}

sf::Shader *ResourcesLoader::getShader(Shader::SourceShader shader) {
    return shaderMap.at(shader);
}
