#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "scene.h"

void Scene::initObjects()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {

            if ((i + j) % 2 == 0)
            {
                Object r = Object(Shape::circleShape, 0, 0.5f, 10.f, 0.5f, 0.3f, sf::Vector2f(300.f + 21.f * i, 300.f + 21.f * j), 10.f);

                r.layer = 1;
                r.circleObject.shape.setFillColor(sf::Color(255, 0, 0));

                objects.push_back(r);
            }

            else
            {
                Object r = Object(Shape::circleShape, 0, 0.5f, 10.f, 0.5f, 0.3f, sf::Vector2f(300.f + 21.f * i, 300.f + 21.f * j), 10.f);

                objects.push_back(r);
            }
        }
    }

    Object r1 = Object(Shape::rectangleShape, 0, 0.5f, 0.f, 0.5f, 0.3f, sf::Vector2f(300.f, 600.f), 400.f, 20.f);

    Object r2 = Object(Shape::rectangleShape, 0, 0.5f, 10.f, 0.5f, 0.3f, sf::Vector2f(300.f, 200.f), 50.f, 20.f);
    Object r3 = Object(Shape::rectangleShape, 0, 0.5f, 20.f, 0.5f, 0.3f, sf::Vector2f(360.f, 200.f), 20.f, 30.f);
    // r3.angularVel = 10;

    objects.push_back(r1);
    objects.push_back(r2);
    objects.push_back(r3);
}

void Scene::initManifolds()
{
    Manifold man;

    for (unsigned int i = 0; i < objects.size() - 1; i++)
    {
        for (unsigned int j = i + 1; j < objects.size(); j++)
        {

            if (objects[i].layer == objects[j].layer)
            {
                man.A = &objects[i];
                man.B = &objects[j];

                manifolds.push_back(man);
            }
        }
    }
}

Scene::Scene(const int _fps, std::vector<Button *> sceneButtons) : mainWindow(sf::VideoMode(800, 800), "main")
{
    static const int fps = _fps;
    dt = float(1. / _fps);

    previousTime = std::chrono::system_clock::now();
    accumulator = 0.;

    alpha = 0.;

    initObjects();
    initManifolds();

    emptyButton = Button();
    mousedButton = &emptyButton;

    playButton = (*sceneButtons[0]);

    circleButton = (*sceneButtons[1]);
    rectangleButton = (*sceneButtons[2]);

    gravityButton = (*sceneButtons[3]);

    refreshButton = (*sceneButtons[4]);

    buttons.push_back(&playButton);

    buttons.push_back(&circleButton);
    oneStateButtons.push_back(&circleButton);

    buttons.push_back(&rectangleButton);
    oneStateButtons.push_back(&rectangleButton);

    buttons.push_back(&gravityButton);
    buttons.push_back(&refreshButton);

    emptyObject = Object();
    toPlaceObject = emptyObject;

    mousedObject = &emptyObject;
    releasedObject = &emptyObject;

    emptyTextBox = TextBox();
    mousedTextBox = &emptyTextBox;
    releasedTextBox = &emptyTextBox;

    addBeginForce = false;
    maxBeginForce = 50;
}

Scene::~Scene() {}

void Scene::updateButtons()
{
    for (unsigned int i = 0; i < buttons.size(); i++)
    {
        if (buttons[i]->handleApp(sf::Mouse::getPosition(mainWindow)))
        {
            mousedButton = buttons[i];
        }

        mainWindow.draw(buttons[i]->app);
    }
}

void Scene::checkEvents(sf::Event event)
{
    if (event.type == sf::Event::Closed)
    {
        mainWindow.close();
    }

    if (event.type == sf::Event::KeyPressed)
    {
        // IF WE ARE WRITING IN TEXTBOX
        if (!releasedTextBox->empty)
        {
            if (event.key.code == sf::Keyboard::Enter)
            {
                try
                {
                    if (releasedObject->rectangleObject.shape.getFillColor().r == 200 || releasedObject->circleObject.shape.getFillColor().r == 200)
                    {
                        throw(std::invalid_argument(""));
                    }

                    float maxVal = releasedTextBox->maxValue;
                    float minVal = releasedTextBox->minValue;

                    if (releasedTextBox->paramNumber == 8)
                    {
                        maxVal = sqrtf(powf(maxBeginForce, 2) - powf(releasedObject->beginForce.y, 2));
                        minVal = -maxVal;
                    }

                    else if (releasedTextBox->paramNumber == 9)
                    {
                        maxVal = sqrtf(powf(maxBeginForce, 2) - powf(releasedObject->beginForce.x, 2));
                        minVal = -maxVal;
                    }

                    float paramValue = validValue(releasedTextBox->tempString, maxVal, minVal);

                    releasedTextBox->finalString = toStringFormat(paramValue);
                    releasedTextBox->tempString = "";

                    releasedTextBox->textField.setString(releasedTextBox->finalString);
                    releasedObject->changeParameter(releasedTextBox->paramNumber, paramValue);

                    releasedObject->reinitTextBoxes();
                    releasedTextBox = &emptyTextBox;
                    tempManifolds = std::vector<Manifold>();
                }

                catch (std::invalid_argument)
                {
                    releasedObject->reinitTextBoxes();
                    releasedTextBox = &emptyTextBox;
                    tempManifolds = std::vector<Manifold>();
                }
            }

            if (event.key.code == sf::Keyboard::BackSpace)
            {
                if (releasedTextBox->tempString.size() > 0)
                {
                    releasedTextBox->tempString = releasedTextBox->tempString.substr(0, releasedTextBox->cursor - 1) + releasedTextBox->tempString.substr(releasedTextBox->cursor, releasedTextBox->tempString.size() - 1);
                    releasedTextBox->cursor -= 1;

                    releasedTextBox->textField.setString(releasedTextBox->tempString);
                }
            }

            if (event.key.code == sf::Keyboard::Delete)
            {
                if (static_cast<std::size_t>(releasedTextBox->cursor) < releasedTextBox->tempString.size())
                {
                    releasedTextBox->tempString = releasedTextBox->tempString.substr(0, releasedTextBox->cursor) + releasedTextBox->tempString.substr(releasedTextBox->cursor + 1, releasedTextBox->tempString.size() - 1);

                    releasedTextBox->textField.setString(releasedTextBox->tempString);
                }
            }

            if (event.key.code == sf::Keyboard::Left)
            {
                releasedTextBox->cursor = std::max(0, releasedTextBox->cursor - 1);
            }

            if (event.key.code == sf::Keyboard::Right)
            {
                releasedTextBox->cursor = std::min((int)releasedTextBox->tempString.size(), releasedTextBox->cursor + 1);
            }
        }

        else
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                playButton.released = !playButton.released;
            }
        }
    }

    // GET TEXT FOR TEXTBOX
    if (event.type == sf::Event::TextEntered)
    {
        if (!releasedTextBox->empty)
        {
            // check if the key pressed is valid
            if (isValidCharacter(event.key.code))
            {
                std::string newChar = "";
                newChar += event.text.unicode;

                releasedTextBox->tempString = releasedTextBox->tempString.substr(0, releasedTextBox->cursor) + newChar + releasedTextBox->tempString.substr(releasedTextBox->cursor, releasedTextBox->tempString.size() - releasedTextBox->cursor);
                ;
                releasedTextBox->cursor += 1;

                releasedTextBox->textField.setString(releasedTextBox->tempString);

                if (releasedTextBox->paramNumber <= 2 || releasedTextBox->paramNumber >= 6)
                {
                    try
                    {
                        float maxVal = releasedTextBox->maxValue;
                        float minVal = releasedTextBox->minValue;

                        if (releasedTextBox->paramNumber == 8)
                        {
                            maxVal = sqrtf(powf(maxBeginForce, 2) - powf(releasedObject->beginForce.y, 2));
                            minVal = -maxVal;
                        }

                        else if (releasedTextBox->paramNumber == 9)
                        {
                            maxVal = sqrtf(powf(maxBeginForce, 2) - powf(releasedObject->beginForce.x, 2));
                            minVal = -maxVal;
                        }

                        float paramValue = validValue(releasedTextBox->tempString, maxVal, minVal);
                        releasedObject->changeParameter(releasedTextBox->paramNumber, paramValue);
                    }

                    catch (std::invalid_argument)
                    {
                    }
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // BUTTONS
            if (!mousedButton->empty)
            {
                mousedButton->clicked = true;
            }

            // TEXTBOX
            else if (!mousedTextBox->empty)
            {
                mousedTextBox->clicked = true;
            }

            // MOUSE OBJECT
            else if (!mousedObject->empty)
            {
                mousedObject->clicked = true;

                mousedObject->circleObject.shape.setOutlineColor(sf::Color(0, 0, 255));
                mousedObject->rectangleObject.shape.setOutlineColor(sf::Color(0, 0, 255));

                for (unsigned int i = 0; i < oneStateButtons.size(); i++)
                {
                    oneStateButtons[i]->released = false;
                }

                releasedObject->released = false;
                releasedObject = &emptyObject;
                toPlaceObject = emptyObject;
                tempManifolds = std::vector<Manifold>();

                Manifold man;

                for (unsigned int i = 0; i < objects.size(); i++)
                {
                    if (objects[i].layer == mousedObject->layer && !objects[i].clicked)
                    {
                        man.A = &objects[i];
                        man.B = &(*mousedObject);

                        tempManifolds.push_back(man);
                    }
                }
            }

            // NOTHING MOUSED
            else
            {
                if (releasedObject->guiBox.checkMouse(sf::Mouse::getPosition(mainWindow)))
                {
                    releasedObject->reinitTextBoxes();

                    releasedTextBox = &emptyTextBox;
                    tempManifolds = std::vector<Manifold>();
                }

                // NO OBJECT TO PLACE
                else if (toPlaceObject.empty)
                {

                    for (unsigned int i = 0; i < oneStateButtons.size(); i++)
                    {
                        oneStateButtons[i]->released = false;
                    }

                    releasedObject->released = false;
                    releasedObject = &emptyObject;
                }

                // TO PLACE OBJECT
                else
                {
                    if (toPlaceObject.circleObject.shape.getFillColor().r != 200 && toPlaceObject.rectangleObject.shape.getFillColor().r != 200)
                    {
                        toPlaceObject.prevPos = toPlaceObject.pos;
                        toPlaceObject.reloadGUIBox();

                        objects.push_back(toPlaceObject);

                        // each time we place a new object, we should reconstruct the whole manifolds vector because the memory adress of objects in the objects
                        // vector can change ( because we change its size and capacity )
                        manifolds = std::vector<Manifold>();

                        Manifold man;

                        for (unsigned int i = 0; i < objects.size() - 1; i++)
                        {
                            for (unsigned int j = i + 1; j < objects.size(); j++)
                            {

                                if (objects[i].layer == objects[j].layer)
                                {
                                    man.A = &objects[i];
                                    man.B = &objects[j];

                                    manifolds.push_back(man);
                                }
                            }
                        }

                        for (unsigned int i = 0; i < oneStateButtons.size(); i++)
                        {
                            oneStateButtons[i]->released = false;
                        }

                        toPlaceObject = emptyObject;
                        tempManifolds = std::vector<Manifold>();
                    }
                }
            }
        }

        if (event.mouseButton.button == sf::Mouse::Right)
        {
            // Right click on an object -> change beginForce
            if (!releasedObject->empty)
            {
                sf::Vector2f mousePos = sf::Vector2f(float(sf::Mouse::getPosition(mainWindow).x), float(sf::Mouse::getPosition(mainWindow).y));
                float lenVect = len(mousePos - releasedObject->beginForceArrow.startPoint);
                sf::Vector2f endPoint = mousePos;

                if (lenVect > maxBeginForce)
                {
                    endPoint = releasedObject->beginForceArrow.startPoint + maxBeginForce / lenVect * (mousePos - releasedObject->beginForceArrow.startPoint);
                }
                releasedObject->beginForceArrow.setEndPoint(endPoint);
                releasedObject->beginForce = releasedObject->beginForceArrow.vector;

                // Change TextBox Values
                releasedObject->guiBox.textBoxes[releasedObject->guiBox.textBoxes.size() - 1].finalString = toStringFormat(releasedObject->beginForce.y);
                releasedObject->guiBox.textBoxes[releasedObject->guiBox.textBoxes.size() - 1].textField.setString(releasedObject->guiBox.textBoxes[releasedObject->guiBox.textBoxes.size() - 1].finalString);

                releasedObject->guiBox.textBoxes[releasedObject->guiBox.textBoxes.size() - 2].finalString = toStringFormat(releasedObject->beginForce.x);
                releasedObject->guiBox.textBoxes[releasedObject->guiBox.textBoxes.size() - 2].textField.setString(releasedObject->guiBox.textBoxes[releasedObject->guiBox.textBoxes.size() - 2].finalString);
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // BUTTONS
            if (!mousedButton->empty)
            {
                mousedButton->clicked = false;

                if (mousedButton->oneState)
                {
                    for (unsigned int i = 0; i < oneStateButtons.size(); i++)
                    {
                        oneStateButtons[i]->released = false;
                    }

                    mousedButton->released = true;
                }

                else
                {
                    mousedButton->released = !mousedButton->released;

                    if (mousedButton == buttons[0])
                    {
                        if (mousedButton->released)
                        {
                            launchPlay();
                        }

                        else
                        {
                            launchPause();
                        }
                    }
                }

                mousedButton = &emptyButton;
                toPlaceObject = emptyObject;
                releasedObject = &emptyObject;
                tempManifolds = std::vector<Manifold>();
            }

            // TEXTBOX
            else if (!mousedTextBox->empty)
            {
                mousedTextBox->clicked = false;

                releasedObject->reinitTextBoxes();

                mousedTextBox->released = true;
                releasedTextBox = mousedTextBox;

                releasedTextBox->textField.setString(releasedTextBox->tempString);

                mousedTextBox = &emptyTextBox;
                toPlaceObject = emptyObject;
                tempManifolds = std::vector<Manifold>();

                for (unsigned int i = 0; i < oneStateButtons.size(); i++)
                {
                    oneStateButtons[i]->released = false;
                }

                // if parameter to change needs to check collision ( ex change of size, or pos )
                if (releasedTextBox->paramNumber <= 2 || releasedTextBox->paramNumber == 6 || releasedTextBox->paramNumber == 7)
                {
                    Manifold man;

                    for (unsigned int i = 0; i < objects.size(); i++)
                    {
                        if (objects[i].layer == releasedObject->layer && !objects[i].released)
                        {
                            man.A = &objects[i];
                            man.B = releasedObject;

                            tempManifolds.push_back(man);
                        }
                    }
                }
            }

            // MOUSE OBJECT
            else if (!mousedObject->empty)
            {
                mousedObject->clicked = false;

                // REINITIALIZE STATES TEXTBOXES OF PREVIOUS RELEASED OBJECT
                if (!releasedObject->empty)
                {
                    releasedObject->reinitTextBoxes();
                    releasedTextBox = &emptyTextBox;
                    tempManifolds = std::vector<Manifold>();
                }

                if (mousedObject->shapeType == rectangleShape)
                {
                    if (mousedObject->rectangleObject.shape.getFillColor().r != 200)
                    {
                        mousedObject->rectangleObject.shape.setOutlineColor(sf::Color(255, 0, 255));

                        mousedObject->released = true;
                        mousedObject->prevPos = mousedObject->pos;

                        releasedObject = mousedObject;
                    }

                    else
                    {
                        mousedObject->pos = mousedObject->prevPos;

                        mousedObject->rectangleObject.minCoords = mousedObject->pos;
                        mousedObject->rectangleObject.maxCoords = mousedObject->pos + sf::Vector2f(mousedObject->rectangleObject.shape.getSize().x, mousedObject->rectangleObject.shape.getPosition().y);

                        mousedObject->rectangleObject.shape.setFillColor(sf::Color(100, 250, 100));

                        // Change TextBox Values
                        mousedObject->guiBox.textBoxes[0].finalString = toStringFormat(mousedObject->pos.x);
                        mousedObject->guiBox.textBoxes[0].textField.setString(mousedObject->guiBox.textBoxes[0].finalString);

                        mousedObject->guiBox.textBoxes[1].finalString = toStringFormat(mousedObject->pos.y);
                        mousedObject->guiBox.textBoxes[1].textField.setString(mousedObject->guiBox.textBoxes[1].finalString);
                    }
                }

                else
                {
                    if (mousedObject->circleObject.shape.getFillColor().r != 200)
                    {
                        mousedObject->circleObject.shape.setOutlineColor(sf::Color(255, 0, 255));

                        mousedObject->released = true;
                        mousedObject->prevPos = mousedObject->pos;

                        releasedObject = mousedObject;
                    }

                    else
                    {
                        mousedObject->pos = mousedObject->prevPos;

                        mousedObject->circleObject.center = mousedObject->pos + sf::Vector2f(mousedObject->circleObject.radius, mousedObject->circleObject.radius);
                        mousedObject->rectangleObject.minCoords = mousedObject->pos;
                        mousedObject->rectangleObject.maxCoords = mousedObject->pos + sf::Vector2f(2 * mousedObject->circleObject.radius, 2 * mousedObject->circleObject.radius);

                        mousedObject->circleObject.shape.setFillColor(sf::Color(100, 250, 100));

                        // Change TextBox Values
                        mousedObject->guiBox.textBoxes[0].finalString = toStringFormat(mousedObject->pos.x);
                        mousedObject->guiBox.textBoxes[0].textField.setString(mousedObject->guiBox.textBoxes[0].finalString);

                        mousedObject->guiBox.textBoxes[1].finalString = toStringFormat(mousedObject->pos.y);
                        mousedObject->guiBox.textBoxes[1].textField.setString(mousedObject->guiBox.textBoxes[1].finalString);
                    }
                }

                mousedObject = &emptyObject;
                tempManifolds = std::vector<Manifold>();
            }
        }
    }
}

void Scene::handlePause()
{
    // MOUSED OBJECTS CLICKED
    if (mousedObject->clicked)
    {

        // RECTANGLE
        if (mousedObject->shapeType == rectangleShape)
        {
            mousedObject->pos = sf::Vector2f(sf::Mouse::getPosition(mainWindow)) - sf::Vector2f(mousedObject->rectangleObject.shape.getSize().x / 2, mousedObject->rectangleObject.shape.getSize().y / 2);
            // mousedObject->prevPos = mousedObject->pos;

            mousedObject->rectangleObject.minCoords = mousedObject->pos;
            mousedObject->rectangleObject.maxCoords = mousedObject->pos + sf::Vector2f(mousedObject->rectangleObject.shape.getSize().x, mousedObject->rectangleObject.shape.getSize().y);

            mousedObject->rectangleObject.shape.setFillColor(sf::Color(100, 250, 100));
            for (unsigned int i = 0; i < tempManifolds.size(); i++)
            {
                if (tempManifolds[i].A->shapeType == rectangleShape)
                {
                    if (rectangleCollision(&tempManifolds[i]))
                    {
                        mousedObject->rectangleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }

                else
                {
                    if (circleRectangleCollision(&tempManifolds[i]))
                    {
                        mousedObject->rectangleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }
            }
        }

        // CIRCLE
        else
        {
            mousedObject->pos = sf::Vector2f(sf::Mouse::getPosition(mainWindow)) - sf::Vector2f(mousedObject->circleObject.radius, mousedObject->circleObject.radius);
            // mousedObject->prevPos = mousedObject->pos;

            mousedObject->circleObject.center = mousedObject->pos + sf::Vector2f(mousedObject->circleObject.radius, mousedObject->circleObject.radius);
            mousedObject->rectangleObject.minCoords = mousedObject->pos;
            mousedObject->rectangleObject.maxCoords = mousedObject->pos + sf::Vector2f(2 * mousedObject->circleObject.radius, 2 * mousedObject->circleObject.radius);

            mousedObject->circleObject.shape.setFillColor(sf::Color(100, 250, 100));
            for (unsigned int i = 0; i < tempManifolds.size(); i++)
            {
                if (tempManifolds[i].A->shapeType == rectangleShape)
                {
                    if (rectangleCircleCollision(&tempManifolds[i]))
                    {
                        mousedObject->circleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }

                else
                {
                    if (circleCollision(&tempManifolds[i]))
                    {
                        mousedObject->circleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }
            }
        }

        // reposition force arrow
        mousedObject->beginForceArrow.startPoint = mousedObject->pos + (mousedObject->rectangleObject.maxCoords - mousedObject->rectangleObject.minCoords) * 0.5f;
        mousedObject->beginForceArrow.setEndPoint(mousedObject->beginForceArrow.startPoint + mousedObject->beginForceArrow.vector);

        mousedObject->forceArrow.startPoint = mousedObject->pos + (mousedObject->rectangleObject.maxCoords - mousedObject->rectangleObject.minCoords) * 0.5f;
        mousedObject->forceArrow.setEndPoint(mousedObject->forceArrow.startPoint + mousedObject->forceArrow.vector);

        // Change TextBox Values
        mousedObject->guiBox.textBoxes[0].finalString = toStringFormat(mousedObject->pos.x);
        mousedObject->guiBox.textBoxes[0].textField.setString(mousedObject->guiBox.textBoxes[0].finalString);

        mousedObject->guiBox.textBoxes[1].finalString = toStringFormat(mousedObject->pos.y);
        mousedObject->guiBox.textBoxes[1].textField.setString(mousedObject->guiBox.textBoxes[1].finalString);
    }

    // MOUSED OBJECTS UNCLICKED
    else
    {
        mousedObject = &emptyObject;

        for (unsigned int i = 0; i < objects.size(); i++)
        {

            if (objects[i].checkMouse(sf::Mouse::getPosition(mainWindow)))
            {
                if (objects[i].shapeType == circleShape && objects[i].circleObject.shape.getOutlineThickness() == 0.f)
                {
                    objects[i].circleObject.shape.setOutlineThickness(2.f);
                    objects[i].circleObject.shape.setOutlineColor(sf::Color(255, 0, 0));
                }

                if (objects[i].shapeType == rectangleShape && objects[i].rectangleObject.shape.getOutlineThickness() == 0.f)
                {
                    objects[i].rectangleObject.shape.setOutlineThickness(2.f);
                    objects[i].rectangleObject.shape.setOutlineColor(sf::Color(255, 0, 0));
                }

                mousedObject = &objects[i];
            }

            else
            {
                if (&objects[i] != releasedObject)
                {
                    objects[i].circleObject.shape.setOutlineThickness(0.f);
                    objects[i].circleObject.shape.setOutlineColor(sf::Color(0, 0, 0));

                    objects[i].rectangleObject.shape.setOutlineThickness(0.f);
                    objects[i].rectangleObject.shape.setOutlineColor(sf::Color(0, 0, 0));

                    objects[i].clicked = false;
                }
            }
        }
    }

    // RELEASED OBJECTS
    if (!releasedObject->empty)
    {
        for (unsigned int j = 0; j < releasedObject->guiBox.textBoxes.size(); j++)
        {
            if (releasedObject->guiBox.textBoxes[j].handleApp(releasedObject->guiBox.appearance.getPosition(), sf::Mouse::getPosition(mainWindow)))
            {
                mousedTextBox = &(releasedObject->guiBox.textBoxes[j]);
            }
        }
    }

    // RELEASED TEXT BOX
    if (!releasedTextBox->empty)
    {
        releasedTextBox->cursorAnimTimer++;
        if (releasedTextBox->cursorAnimTimer > 2000)
        {
            releasedTextBox->cursorAnimTimer = 0;
        }

        // cases between objects shape
        if (releasedObject->shapeType == rectangleShape)
        {
            releasedObject->rectangleObject.shape.setFillColor(sf::Color(100, 250, 100));

            // we check the temp manifolds in case of resizing or replacing an object
            for (unsigned int i = 0; i < tempManifolds.size(); i++)
            {
                if (tempManifolds[i].A->shapeType == rectangleShape)
                {
                    if (rectangleCollision(&tempManifolds[i]))
                    {
                        releasedObject->rectangleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }

                else
                {
                    if (circleRectangleCollision(&tempManifolds[i]))
                    {
                        releasedObject->rectangleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }
            }
        }

        else
        {
            releasedObject->circleObject.shape.setFillColor(sf::Color(100, 250, 100));

            // we check the temp manifolds in case of resizing or replacing an object
            for (unsigned int i = 0; i < tempManifolds.size(); i++)
            {
                if (tempManifolds[i].A->shapeType == rectangleShape)
                {
                    if (rectangleCircleCollision(&tempManifolds[i]))
                    {
                        releasedObject->circleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }

                else
                {
                    if (circleCollision(&tempManifolds[i]))
                    {
                        releasedObject->circleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }
            }
        }
    }

    // REFRESH BUTTON
    if (refreshButton.clicked && objects.size() > 0)
    {
        objects = std::vector<Object>();
        manifolds = std::vector<Manifold>();

        releasedObject = &emptyObject;
        tempManifolds = std::vector<Manifold>();

        releasedTextBox = &emptyTextBox;
    }

    // TO PLACE OBJECT
    if (circleButton.released && mousedButton->empty)
    {

        if (toPlaceObject.empty)
        {
            toPlaceObject = Object(Shape::circleShape, 0, 0.5f, 10.f, 0.5f, 0.3f, sf::Vector2f(sf::Mouse::getPosition(mainWindow)) - sf::Vector2f(10.f, 10.f), 10.f);

            Manifold man;

            for (unsigned int i = 0; i < objects.size(); i++)
            {
                if (objects[i].layer == toPlaceObject.layer)
                {
                    man.A = &objects[i];
                    man.B = &toPlaceObject;

                    tempManifolds.push_back(man);
                }
            }
        }

        else
        {
            toPlaceObject.circleObject.center = toPlaceObject.pos + sf::Vector2f(toPlaceObject.circleObject.radius, toPlaceObject.circleObject.radius);
            toPlaceObject.rectangleObject.minCoords = toPlaceObject.pos;
            toPlaceObject.rectangleObject.maxCoords = toPlaceObject.pos + sf::Vector2f(2 * toPlaceObject.circleObject.radius, 2 * toPlaceObject.circleObject.radius);

            toPlaceObject.circleObject.shape.setFillColor(sf::Color(100, 250, 100));

            for (unsigned int i = 0; i < tempManifolds.size(); i++)
            {
                if (tempManifolds[i].A->shapeType == rectangleShape)
                {
                    if (rectangleCircleCollision(&tempManifolds[i]))
                    {
                        toPlaceObject.circleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }

                else
                {
                    if (circleCollision(&tempManifolds[i]))
                    {
                        toPlaceObject.circleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }
            }

            toPlaceObject.pos = sf::Vector2f(sf::Mouse::getPosition(mainWindow)) - sf::Vector2f(toPlaceObject.circleObject.radius, toPlaceObject.circleObject.radius);
            toPlaceObject.circleObject.shape.setPosition(toPlaceObject.pos);
            mainWindow.draw(toPlaceObject.circleObject.shape);
        }
    }

    if (rectangleButton.released && mousedButton->empty)
    {

        if (toPlaceObject.empty)
        {
            toPlaceObject = Object(Shape::rectangleShape, 0, 0.5f, 10.f, 0.5f, 0.3f, sf::Vector2f(sf::Mouse::getPosition(mainWindow)) - sf::Vector2f(50.f, 50.f), 50.f, 50.f);

            Manifold man;

            for (unsigned int i = 0; i < objects.size(); i++)
            {
                if (objects[i].layer == toPlaceObject.layer)
                {
                    man.A = &objects[i];
                    man.B = &toPlaceObject;

                    tempManifolds.push_back(man);
                }
            }
        }

        else
        {
            toPlaceObject.rectangleObject.minCoords = toPlaceObject.pos;
            toPlaceObject.rectangleObject.maxCoords = toPlaceObject.pos + sf::Vector2f(toPlaceObject.rectangleObject.shape.getSize().x, toPlaceObject.rectangleObject.shape.getSize().y);

            toPlaceObject.rectangleObject.shape.setFillColor(sf::Color(100, 250, 100));

            for (unsigned int i = 0; i < tempManifolds.size(); i++)
            {
                if (tempManifolds[i].A->shapeType == rectangleShape)
                {
                    if (rectangleCollision(&tempManifolds[i]))
                    {
                        toPlaceObject.rectangleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }

                else
                {
                    if (circleRectangleCollision(&tempManifolds[i]))
                    {
                        toPlaceObject.rectangleObject.shape.setFillColor(sf::Color(200, 250, 200));
                    }
                }
            }

            toPlaceObject.pos = sf::Vector2f(sf::Mouse::getPosition(mainWindow)) - sf::Vector2f(toPlaceObject.rectangleObject.shape.getSize().x / 2, toPlaceObject.rectangleObject.shape.getSize().y / 2);
            toPlaceObject.rectangleObject.shape.setPosition(toPlaceObject.pos);
            mainWindow.draw(toPlaceObject.rectangleObject.shape);
        }
    }
}

void Scene::renderPhysics()
{
    for (unsigned int i = 0; i < manifolds.size(); i++)
    {
        bool isColliding = false;

        switch (manifolds[i].A->shapeType)
        {
        case rectangleShape:
            switch (manifolds[i].B->shapeType)
            {
            case rectangleShape:
                isColliding = rectangleCollision(&manifolds[i]);
                break;

            case circleShape:
                isColliding = rectangleCircleCollision(&manifolds[i]);
                break;
            }
            break;

        case circleShape:
            switch (manifolds[i].B->shapeType)
            {
            case rectangleShape:
                isColliding = circleRectangleCollision(&manifolds[i]);
                break;

            case circleShape:
                isColliding = circleCollision(&manifolds[i]);
                break;
            }
            break;
        }

        if (isColliding)
        {
            resolveCollision(&manifolds[i]);
        }
    }

    for (unsigned int i = 0; i < objects.size(); i++)
    {
        objects[i].applyForce(dt, gravity, addBeginForce);
    }
}

void Scene::handlePlay()
{
    auto currentTime = std::chrono::system_clock::now();
    auto timeDiff = currentTime - previousTime;

    accumulator += timeDiff.count() / 1000000.;

    if (accumulator > 0.2)
    {
        accumulator = 0.2;
    }

    previousTime = currentTime;

    while (accumulator > dt)
    {
        renderPhysics();
        accumulator -= dt;
    }

    alpha = float(accumulator / dt);

    if (addBeginForce)
    {
        addBeginForce = false;
    }
}

void Scene::launchPlay()
{
    releasedObject->rectangleObject.shape.setOutlineThickness(0.f);
    releasedObject->rectangleObject.shape.setOutlineColor(sf::Color(0, 0, 0));

    releasedObject->circleObject.shape.setOutlineThickness(0.f);
    releasedObject->circleObject.shape.setOutlineColor(sf::Color(0, 0, 0));

    releasedObject->reinitTextBoxes();

    releasedObject->released = false;
    releasedObject = &emptyObject;
    tempManifolds = std::vector<Manifold>();

    // i begins at 1 because buttons[0] is playButton
    for (unsigned int i = 1; i < oneStateButtons.size(); i++)
    {
        oneStateButtons[i]->released = false;
    }

    addBeginForce = true;
}

void Scene::launchPause()
{
    alpha = 0.;

    for (unsigned int i = 0; i < objects.size(); i++)
    {
        objects[i].prevPos = objects[i].pos;

        objects[i].beginForceArrow.startPoint = objects[i].pos + (objects[i].rectangleObject.maxCoords - objects[i].rectangleObject.minCoords) * 0.5f;
        objects[i].beginForceArrow.setEndPoint(objects[i].beginForceArrow.startPoint + objects[i].beginForceArrow.vector);

        objects[i].forceArrow.startPoint = objects[i].pos + (objects[i].rectangleObject.maxCoords - objects[i].rectangleObject.minCoords) * 0.5f;
        objects[i].forceArrow.setEndPoint(objects[i].forceArrow.startPoint + objects[i].velocity);
    }
}

void Scene::displayObjects()
{

    if (!releasedObject->empty && !playButton.released)
    {
        releasedObject->guiBox.display(mainWindow, releasedObject->pos + releasedObject->rectangleObject.maxCoords - releasedObject->rectangleObject.minCoords);
    }

    for (unsigned int i = 0; i < objects.size(); i++)
    {
        if (objects[i].shapeType == circleShape)
        {
            objects[i].circleObject.shape.setPosition(alpha * objects[i].prevPos + (1 - alpha) * objects[i].pos);
            mainWindow.draw(objects[i].circleObject.shape);
        }

        else
        {
            objects[i].rectangleObject.shape.setPosition(alpha * objects[i].prevPos + (1 - alpha) * objects[i].pos);
            sf::Transform test;
            test.rotate(objects[i].orientation, objects[i].rectangleObject.shape.getPosition().x + objects[i].rectangleObject.shape.getSize().x / 2, objects[i].rectangleObject.shape.getPosition().y + objects[i].rectangleObject.shape.getSize().y / 2);

            mainWindow.draw(objects[i].rectangleObject.shape, test);
        }

        if (!playButton.released)
        {
            objects[i].beginForceArrow.draw(mainWindow);
            objects[i].forceArrow.draw(mainWindow);
        }
    }
}

void Scene::replayLoop()
{
    mousedButton = &emptyButton;
    mousedTextBox = &emptyTextBox;

    gravity = sf::Vector2f(0.f, 9.81f * float(gravityButton.released));
}

bool Scene::getActiveState()
{
    return (playButton.released);
}
