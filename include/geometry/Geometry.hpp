#pragma once

#include <SFML/Graphics.hpp>
#include "graphics/ResourcesLoader.hpp"

/*
    C++ doesn't provides a way to extend a class wihout inheritance.
    This class is a set of basic geometric operation necessary to works with
    basic types of sfml. 
*/
class Geometry {
    public:
        static sf::Vector2f floatRectPosition(sf::FloatRect rect);
        static sf::Vector2f floatRectSize(sf::FloatRect rect);
        static sf::FloatRect rectangleShapeToFloatRect(sf::RectangleShape shape);
        static void applyFloatRectToRectangleShape(
            sf::RectangleShape & shape, sf::FloatRect const rect
            );

        /*
            Scale rect from center.
        */
        static sf::FloatRect scaleRect(sf::FloatRect rect, float factor);
        static sf::FloatRect scaleRect(sf::FloatRect rect, float factor, sf::Vector2f origin);

        /*
            Minimal shape that respects resolution ratio, and contains targetShape.
        */
        static sf::FloatRect fitOutside(sf::Vector2f resolution, 
            sf::Vector2f targetShapePosition,
            sf::Vector2f targetShapeSize
            );
        static sf::FloatRect fitOutside(
            sf::Vector2f resolution, sf::FloatRect targetShape);

        /*
            Maximal shape that respects resolution ratio, and contained in targetShape
        */
        static sf::FloatRect fitInside(sf::Vector2f resolution,
            sf::Vector2f targetShapePosition,
            sf::Vector2f targetShapeSize
            );
        static sf::FloatRect fitInside(
            sf::Vector2f resolution, sf::FloatRect targetShape);

        static sf::Vector2f toFloat(sf::Vector2u v);
        static sf::Vector2f toFloat(sf::Vector2i v);

        /*
            Generate width / height ratio.
        */
        static float toRatio(sf::Vector2f v);
        static float toRatio(sf::Vector2u v);
        static float toRatio(sf::Vector2i v);
        static float toRatio(sf::FloatRect rect);
        static float toRatio(Texture::SourceTexture texture);

        /*
            Calculate the maximal rectangle contained in both.
            Throw error if it doe not exists.
        */
        static sf::FloatRect minRectangle(sf::FloatRect a, sf::FloatRect b);
        static sf::FloatRect minRectangle(sf::RectangleShape a, sf::RectangleShape b);

        static sf::Vector2f spaceTransform(
            sf::Vector2f vector, sf::FloatRect oldSpace, sf::FloatRect newSpace);
        static sf::FloatRect spaceTransform(
            sf::FloatRect rect, sf::FloatRect oldSpace, sf::FloatRect newSpace);

        static bool insideUnitBox(sf::Vector2f v);

        static sf::FloatRect worldSpaceForShader(sf::RenderWindow *window);
};