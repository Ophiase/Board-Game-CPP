#include "graphics/Launcher.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "utils/Cli.hpp"
#include "geometry/Geometry.hpp"
#include "graphics/screen/Screen.hpp"
#include "graphics/Menu.hpp"


// --------------------------------------------------------
// PUBLIC

Launcher::Launcher() : window {
        sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y), DEFAULT_WINDOW_TITLE, DEFAULT_STYLE,
} {
    //this->window.setVerticalSyncEnabled(true);
    this->window.setFramerateLimit(60);
    this->window.setVisible(false);

    ResourcesLoader::initialize();
    if (!ResourcesLoader::load()) {
        std::cerr << "Error loading textures!" << std::endl;
        exit(1);
    };
    
    Cli::info("Start Launcher");
}

sf::FloatRect Launcher::getScreen() const {
    return Geometry::rectangleShapeToFloatRect(this->screen);
}


sf::FloatRect Launcher::getRenderZone() const {
    return Geometry::minRectangle(this->maxRenderZone, this->screen);
}
sf::RenderWindow & Launcher::getRenderWindow() {
    return this->window;
}

sf::RenderWindow const & Launcher::getConstRenderWindow() const {
    return this->window;
}

bool Launcher::run() {
    {
        /* critical section*/
        isRunningMutex.lock();

        if (isRunning) 
            return false;
        isRunning = true;
        
        isRunningMutex.unlock();
    }

    this->window.setVisible(true);

    Menu menu{this};
    focus = &menu;
    
    this->initView();
    this->updateView();

    while (this->window.isOpen()) {
        if (!focus->isAlive() && (focus != &menu)) {
            delete focus;
            focus = &menu;
        };

        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();

            if (event.type == sf::Event::Resized) {
                this->adjustSize();
                this->updateView();
            }

            focus->handleEvent(event);

            if (focus == &menu)
                if (menu.successor) {
                    focus = menu.successor;
                    menu.successor = nullptr; 
                }
        }

        this->window.clear(sf::Color::Black);
        this->window.draw(background);

        this->window.draw(foreground);
        
        this->window.draw(title);
        
        focus->draw();

        applyLimitMask();
        this->window.display();
    }

    if (focus != &menu)
        delete focus;

    return true;
}

// --------------------------------------------------------
// PRIVATE

void Launcher::initView() {
    
    {
        maxRenderZone = sf::RectangleShape{sf::Vector2f{2.0f, 1.0f}};
        maxRenderZone.setPosition(sf::Vector2f{
            0.5f - (maxRenderZone.getSize().x / 2.0f), 
            0.4f - (maxRenderZone.getSize().y / 2.0f)
        });
    }

    {
        float titleRatio = Geometry::toRatio(Texture::BackgroundPanelTop);
        title = sf::RectangleShape(sf::Vector2f(1.0, 1.0/titleRatio));
        title.setTexture(ResourcesLoader::getTexture(Texture::BackgroundPanelTop));
    }

    {
        sf::Vector2f center{0.5, 0.125};

        title.setPosition(0, -title.getSize().y);
        title.setPosition(0, title.getPosition().y + center.y);

        auto transform = Geometry::scaleRect(
            Geometry::rectangleShapeToFloatRect(title),
            2.1,
            center
        );
        
        Geometry::applyFloatRect(
            title, transform
        );
    }

    {
        float backgroundRatio = Geometry::toRatio(Texture::BackgroundMainBlur);
        background = sf::RectangleShape(sf::Vector2f(1.0, 1.0/backgroundRatio));
        background.setTexture(ResourcesLoader::getTexture(Texture::BackgroundMainBlur));
    }
    
    {
        float foregroundRatio = Geometry::toRatio(Texture::BackgroundMenu2);
        foreground = sf::RectangleShape(sf::Vector2f(1.0, 1/foregroundRatio));
        foreground.setTexture(ResourcesLoader::getTexture(Texture::BackgroundMenu2));
        
        float freeYSpace = 1.0f - (1.0f / foregroundRatio);    
        foreground.setPosition(0.0, freeYSpace*0.3);
    }
}

void Launcher::updateView() {

    (void)title;

    float inBiais = 0.04f;
    float foregroundBottom = foreground.getSize().y + foreground.getPosition().y - inBiais;
    sf::FloatRect visibleArea = Geometry::fitOutside(
        Geometry::toFloat(this->window.getSize()), 
        sf::Vector2f{0.0, 0.0},
        sf::Vector2f{1.0, foregroundBottom}
    );

    Geometry::applyFloatRect(this->screen, visibleArea);

    auto transform = Geometry::fitOutside(background.getSize(), visibleArea);
    Geometry::applyFloatRect(
        background, transform
    );

    this->window.setView(sf::View(visibleArea));

}

void Launcher::adjustSize() {
    /*
        This code section was meant to impose limits
        on window size and aspect ratio.

        A bug on linux makes it impossible : 
        https://github.com/SFML/SFML/issues/2124
        
        sf::Vector2u size{
            std::max(this->window.getSize().x, MINIMUM_WINDOW_SIZE.x),
            std::max(this->window.getSize().y, MINIMUM_WINDOW_SIZE.y),
        };
        this->window.setSize(size);
    */
}

void Launcher::applyLimitMask() {
    float ratio = (float)window.getSize().x / screen.getSize().x;
    auto origin = maxRenderZone.getPosition() - screen.getPosition();
    origin.x *= ratio;
    origin.y *= ratio;

    // origin of frag shader is on bottom left, not top left ...
    origin.y = (float)window.getSize().y - origin.y;
    
    auto size = maxRenderZone.getSize();
    size.x *= ratio;
    size.y *= -ratio;

    sf::Shader *maskShader = ResourcesLoader::getShader(Shader::Mask);
    maskShader->setUniform("origin", origin);
    maskShader->setUniform("size", size);
    
    /*
    maskShader->setUniform("worldSpace", sf::Glsl::Vec4(
        origin.x, origin.y,
        size.x, size.y
        ));
    */
    
    this->window.draw(
        this->screen, maskShader);
}