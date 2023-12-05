#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "engine/Cell.hpp"
#include "engine/Side.hpp"

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
        TileWood,

        CheckerBoard0,
        CheckerBoard1,
        CheckerBoard2,
        CheckerBoard3,

        PawnBlack,
        PawnBlackBis,
        PawnWhite,
        PawnWhiteBis,
        PawnRed,
        PawnGreen,
        PawnYellow,
        PawnBlue
    };
}
namespace Font {
    /*
        Linked to resources into ResourcesLoader::initialize.
    */
    enum SourceFont {
        OpenSansBold,
        OpenSansExtraBold,
        OpenSansRegular,
        OpenSansLight,
        OpenSansSemiBold
    };
}

namespace Shader {
    /*
        Linked to resources into ResourcesLoader::initialize.
    */
    enum SourceShader {
        Test,
        Mask,
        Coord,
    };
}


class ResourcesLoader {
private:
    static bool _loaded;

    static std::map<Texture::SourceTexture, std::string> texturePathMap;
    static std::map<Texture::SourceTexture, sf::Texture*> textureMap;
    static void initializeTextures();
    static bool loadTextures();

    static std::map<char, std::string> charTexturePathMap;
    static std::map<char, sf::Texture*> charTextureMap;
    static void initializeCharTextures();
    static bool loadCharTextures();

    static std::map<Font::SourceFont, std::string> fontPathMap;
    static std::map<Font::SourceFont, sf::Font*> fontMap;
    static void initializeFonts();
    static bool loadFonts();

    static std::map<Shader::SourceShader, std::string> shaderPathMap;
    static std::map<Shader::SourceShader, sf::Shader*> shaderMap;
    static void initializeShaders();
    static bool loadShaders();

    static Texture::SourceTexture toSourceTexture(CellPiece piece);
    static Texture::SourceTexture toSourceTexture(SidePiece piece);

public:
    static void initialize();
    static bool load();
    static bool loaded();
    //static bool unload(); // TODO, dealocate

    static sf::Texture *getTexture(Texture::SourceTexture texture);
    static sf::Texture *getCharTexture(char c, bool unsafe=false);
    static sf::Texture *getTexture(CellPiece piece);
    static sf::Texture *getTexture(SidePiece piece);

    static sf::Font *getFont(Font::SourceFont font);
    static sf::Shader *getShader(Shader::SourceShader shader);
};