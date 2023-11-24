#pragma once

#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "struct.h"
#include "button.h"
#include "physics.h"

class Scene {
public:
    sf::RenderWindow mainWindow;

    void initObjects();
    void initManifolds();

    Scene(const int _fps, std::vector<Button*> sceneButtons);
    ~Scene();

	void updateButtons();
    void checkEvents(sf::Event event);

    void handlePause();
    void handlePlay();

    void launchPlay();
    void launchPause();

    void displayObjects();
    void replayLoop();

    bool getActiveState();
    

private:
	float dt;
	float alpha;
	double accumulator;
    std::chrono::system_clock::time_point previousTime;
    sf::Vector2f gravity;
    bool addBeginForce;
    float maxBeginForce;

    std::vector<Button*> buttons;
    std::vector<Button*> oneStateButtons;

    Button emptyButton;
    Button* mousedButton;

    Button playButton;
    Button refreshButton;
    Button gravityButton;
    Button circleButton;
    Button rectangleButton;

    std::vector<Object> objects;
    std::vector<Manifold> manifolds;

    Object emptyObject;
    Object toPlaceObject;
    std::vector<Manifold> tempManifolds;

    Object* mousedObject;
    Object* releasedObject;

    TextBox* mousedTextBox;
    TextBox* releasedTextBox;
    TextBox emptyTextBox;

    void renderPhysics();
};

