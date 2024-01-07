#include "graphics/ResourcesLoader.hpp"

#include "utils/Cli.hpp"
#include "utils/NotImplemented.hpp"
#include "stdexcept"

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
        make_tuple(TileWheatBis, "tile/wheat_bis"),
        make_tuple(TileWood, "tile/wood"),

        make_tuple(CheckerBoard0, "checkerboard/checkerboard_0"),
        make_tuple(CheckerBoard1, "checkerboard/checkerboard_1"),
        make_tuple(CheckerBoard2, "checkerboard/checkerboard_2"),
        make_tuple(CheckerBoard3, "checkerboard/checkerboard_3"),
        
        make_tuple(PawnBlack, "entity/pawn_black"),
        make_tuple(PawnBlackBis, "entity/pawn_black_bis"),
        make_tuple(PawnWhite, "entity/pawn_white"),
        make_tuple(PawnWhiteBis, "entity/pawn_white_bis"),

        make_tuple(QueenWhite, "entity/queen_white"),
        make_tuple(QueenBlack, "entity/queen_black"),

        make_tuple(KingWhite, "entity/king_white"),
        make_tuple(KingBlack, "entity/king_black"),

        make_tuple(PawnRed, "entity/pawn_red_bis"),
        make_tuple(PawnGreen, "entity/pawn_green"),
        make_tuple(PawnYellow, "entity/pawn_yellow_bis"),
        make_tuple(PawnBlue, "entity/pawn_blue"),

        make_tuple(SideWhite, "entity/side_white"),
        make_tuple(SideBlack, "entity/side_black"),
        make_tuple(SideQueenWhite, "entity/side_queen_white"),
        make_tuple(SideQueenBlack, "entity/side_queen_black"),

        make_tuple(Arrow, "entity/arrow_bis"),
        make_tuple(Selection, "entity/circle_selection"),
        make_tuple(Selectable, "entity/selectable")
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
        associations.push_back(make_tuple(
            c, "d" + std::string(1, c)
            ));

    // uppercase
    for (char c = 'A'; c < 'Z'; c++)
        associations.push_back(make_tuple(
            c, "u_" + std::string(1, std::tolower(c))
            ));

    // lowercase
    for (char c = 'a'; c < 'z'; c++)
        associations.push_back(make_tuple(
            c, "l_" + std::string(1, c)
        ));

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
    Cli::debug("- Load Texture");
    for (const auto& pair : texturePathMap) {
        auto texture = new sf::Texture{};
        if (!texture->loadFromFile(pair.second))
            return false;
        textureMap[pair.first] = texture;
    }
    return true;    
}

bool ResourcesLoader::loadCharTextures() {
    Cli::debug("- Load CharTexture");
    for (const auto& pair : charTexturePathMap) {
        auto charTexture = new sf::Texture{};
        if (!charTexture->loadFromFile(pair.second))
            return false;
        charTextureMap[pair.first] = charTexture;
    }
    return true;    
}

bool ResourcesLoader::loadFonts() {
    Cli::debug("- Load Fonts");
    for (const auto& pair : fontPathMap) {
        auto font = new sf::Font{};
        if (!font->loadFromFile(pair.second))
            return false;
        fontMap[pair.first] = font;
    }
    return true;    
}

bool ResourcesLoader::loadShaders() {
    Cli::debug("- Load Shaders");
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


Texture::SourceTexture ResourcesLoader::toSourceTexture(CellPiece piece) {
    switch(piece.pieceType) {
        case CellPieceType::BlackPawn : return Texture::PawnBlack; 
        case CellPieceType::WhitePawn : return Texture::PawnWhite; 
        
        case CellPieceType::BlackQueen : return Texture::QueenBlack; 
        case CellPieceType::WhiteQueen : return Texture::QueenWhite;

        case CellPieceType::BlackKing : return Texture::KingBlack; 
        case CellPieceType::WhiteKing : return Texture::KingWhite;

        case CellPieceType::YellowPawn : return Texture::PawnYellow;
        case CellPieceType::RedPawn : return Texture::PawnRed;

        default:
            throw std::invalid_argument("Invalid piece type.");
    }
}

Texture::SourceTexture ResourcesLoader::toSourceTexture(SidePiece piece) {
    switch(piece.pieceType) {
        case SidePieceType::BlackSidePawn : return Texture::SideBlack;
        case SidePieceType::WhiteSidePawn : return Texture::SideWhite;
        case SidePieceType::BlackSideQueen : return Texture::SideQueenBlack;
        case SidePieceType::WhiteSideQueen : return Texture::SideQueenWhite;

        default:
            throw std::invalid_argument("Invalid piece type.");
    }
}

sf::Texture *ResourcesLoader::getTexture(Texture::SourceTexture texture) {
    return textureMap.at(texture);
}

sf::Texture *ResourcesLoader::getTexture(CellPiece piece) {
    return getTexture(toSourceTexture(piece));
}

sf::Texture *ResourcesLoader::getTexture(SidePiece piece) {
    return getTexture(toSourceTexture(piece));
}

sf::Texture *ResourcesLoader::getCharTexture(char c, bool unsafe) {
    if (unsafe)
        return charTextureMap.at(c);
    
    return (charTextureMap.find(c) != charTextureMap.end()) ? 
        charTextureMap.at(c) : 
        charTextureMap.at('?');
}

sf::Font *ResourcesLoader::getFont(Font::SourceFont font) {
    return fontMap.at(font);
}

sf::Shader *ResourcesLoader::getShader(Shader::SourceShader shader) {
    return shaderMap.at(shader);
}
