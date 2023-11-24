// 2D-Physics-Engine.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "scene.h"

std::string playButtonSprites[6] = {"resources/sprites/play.png", "resources/sprites/playMouse.png", "resources/sprites/playClick.png", "resources/sprites/pause.png", "resources/sprites/pauseMouse.png", "resources/sprites/pauseClick.png"};
Button playButton = Button(playButtonSprites, sf::Vector2f(700, 20), false);

std::string circleButtonSprites[6] = {"resources/sprites/circle.png", "resources/sprites/circleMouse.png", "resources/sprites/circleRelease.png", "resources/sprites/circleRelease.png", "resources/sprites/circleRelease.png", "resources/sprites/circleRelease.png"};
Button circleButton = Button(circleButtonSprites, sf::Vector2f(20, 20), true);

std::string rectangleButtonSprites[6] = {"resources/sprites/rectangle.png", "resources/sprites/rectangleMouse.png", "resources/sprites/rectangleRelease.png", "resources/sprites/rectangleRelease.png", "resources/sprites/rectangleRelease.png", "resources/sprites/rectangleRelease.png"};
Button rectangleButton = Button(rectangleButtonSprites, sf::Vector2f(140, 20), true);

std::string gravityButtonSprites[6] = {"resources/sprites/gravity.png", "resources/sprites/gravityMouse.png", "resources/sprites/gravityClick.png", "resources/sprites/gravityRelease.png", "resources/sprites/gravityMouseRelease.png", "resources/sprites/gravityClickRelease.png"};
Button gravityButton = Button(gravityButtonSprites, sf::Vector2f(260, 20), false);

std::string refreshButtonSprites[6] = {"resources/sprites/refresh.png", "resources/sprites/refreshMouse.png", "resources/sprites/refreshClick.png", "resources/sprites/refresh.png", "resources/sprites/refreshMouse.png", "resources/sprites/refreshClick.png"};
Button refreshButton = Button(refreshButtonSprites, sf::Vector2f(380, 25), false);

std::vector<Button *> sceneButtons = {&playButton, &circleButton, &rectangleButton, &gravityButton, &refreshButton};

std::string textBoxSprites[6] = {"resources/sprites/textBox.png", "resources/sprites/textBoxMouse.png", "resources/sprites/textBoxClick.png", "resources/sprites/textBoxRelease.png", "resources/sprites/textBoxRelease.png", "resources/sprites/textBoxRelease.png"};
sf::Texture textBoxMainTexture;
bool loadTest = textBoxMainTexture.loadFromFile(textBoxSprites[0]);
sf::Texture textBoxMouseTexture;
bool loadTest2 = textBoxMouseTexture.loadFromFile(textBoxSprites[1]);
sf::Texture textBoxClickTexture;
bool loadTest3 = textBoxClickTexture.loadFromFile(textBoxSprites[2]);
sf::Texture textBoxReleaseTexture;
bool loadTest4 = textBoxReleaseTexture.loadFromFile(textBoxSprites[3]);
sf::Texture textures[6] = {textBoxMainTexture, textBoxMouseTexture, textBoxClickTexture, textBoxReleaseTexture, textBoxReleaseTexture, textBoxReleaseTexture};

sf::Font textBoxFont;
bool loadTest5 = textBoxFont.loadFromFile("resources/fonts/arial.ttf");
sf::Font fonts[1] = {textBoxFont};

Scene mainScene(144, sceneButtons);

int main()
{
    while (mainScene.mainWindow.isOpen())
    {

        sf::Event event;

        mainScene.mainWindow.clear();

        mainScene.updateButtons();

        if (mainScene.getActiveState())
        {
            mainScene.handlePlay();
        }

        else
        {
            mainScene.handlePause();
        }

        while (mainScene.mainWindow.pollEvent(event))
        {
            mainScene.checkEvents(event);
        }

        mainScene.displayObjects();

        mainScene.mainWindow.display();

        mainScene.replayLoop();
    }

    return (0);
}
