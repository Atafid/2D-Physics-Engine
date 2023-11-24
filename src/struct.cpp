#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#define PI 3.141592653589793238462643383279502884197169399
#include "struct.h"
#include "physics.h"
#include "functions.h"

// ARROW CLASS
Arrow::Arrow(sf::Vector2f _startPoint, sf::Vector2f _endPoint, float _thickness, sf::Color _color)
{
    startPoint = _startPoint;
    endPoint = _endPoint;
    vector = _endPoint - _startPoint;

    thickness = _thickness;
    length = len(_endPoint - _startPoint);

    tail = sf::RectangleShape(sf::Vector2f(length, _thickness));
    triangle = sf::CircleShape(2 * _thickness, 3);

    tail.setPosition(_startPoint);
    tail.setFillColor(_color);

    triangle.setPosition(_endPoint);
    triangle.setFillColor(_color);

    angle = float((180 / PI) * atanf((_endPoint.y - _startPoint.y) / (_endPoint.x - _startPoint.x)));

    tail.setOrigin(sf::Vector2f(0.f, tail.getSize().y / 2));
    tail.setRotation(angle);

    triangle.setOrigin(sf::Vector2f(triangle.getRadius(), triangle.getRadius()));
    triangle.setRotation(angle + 90);
}

void Arrow::setEndPoint(sf::Vector2f _endPoint)
{
    endPoint = _endPoint;
    vector = endPoint - startPoint;

    length = len(endPoint - startPoint);

    tail.setSize(sf::Vector2f(length, thickness));

    if (vector.x >= 0)
    {
        angle = float((180 / PI) * atanf((_endPoint.y - startPoint.y) / (_endPoint.x - startPoint.x)));
    }

    else
    {
        angle = float((180 / PI) * atanf((_endPoint.y - startPoint.y) / (_endPoint.x - startPoint.x))) + 180;
    }

    tail.setRotation(angle);
    triangle.setRotation(angle + 90);
}

void Arrow::draw(sf::RenderTarget &target)
{
    tail.setPosition(startPoint);
    triangle.setPosition(endPoint);

    target.draw(tail);

    if (length != 0)
    {
        target.draw(triangle);
    }
}

// GUIBOX CLASS
GUIBox::GUIBox(Shape _shape, float xsize, float ysize, float resti, float mass, float friction, float xPos, float yPos)
{
    objectShape = _shape;
    appearance = sf::RectangleShape(sf::Vector2f(100.f, 330.f));
    appearance.setFillColor(sf::Color(50, 50, 50));

    textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "x position :", xPos, 6, 800.f, 0.f));
    textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "y position :", yPos, 7, 800.f, 0.f));

    if (objectShape == rectangleShape)
    {
        textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "x size :", xsize, 0, 800.f, 0.1f));
        textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "y size :", ysize, 1, 800.f, 0.1f));
    }

    else
    {
        textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "radius :", xsize, 2, 400.f, 0.1f));
    }

    textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "restitution :", resti, 3, 10.f, 0.f));
    textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "mass :", mass, 4, 10000.f, 0.f));
    textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "friction :", friction, 5, 1000.f, 0.f));

    textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "x force :", 0.f, 8, 500.f, -500.f));
    textBoxes.push_back(TextBox(textures, sf::Vector2f(10.f, 10.f + textBoxes.size() * 40.f), fonts, "y force :", 0.f, 9, 500.f, -500.f));
}

void GUIBox::display(sf::RenderWindow &targetWindow, sf::Vector2f coords)
{
    appearance.setPosition(coords);
    targetWindow.draw(appearance);

    for (unsigned int i = 0; i < textBoxes.size(); i++)
    {
        textBoxes[i].app.setPosition(coords + textBoxes[i].coordsGUIWorld);
        textBoxes[i].textLabel.setPosition(coords + textBoxes[i].coordsGUIWorld - sf::Vector2f(0.f, 10.f));
        textBoxes[i].textField.setPosition(coords + textBoxes[i].coordsGUIWorld + sf::Vector2f(0.f, -3.f));

        targetWindow.draw(textBoxes[i].app);

        if (textBoxes[i].released && textBoxes[i].cursorAnimTimer >= 1000)
        {
            textBoxes[i].cursorApp.setPosition(textBoxes[i].textField.findCharacterPos(textBoxes[i].cursor) + sf::Vector2f(-1.f, 2.f));
            targetWindow.draw(textBoxes[i].cursorApp);
        }
        targetWindow.draw(textBoxes[i].textLabel);
        targetWindow.draw(textBoxes[i].textField);
    }
}

bool GUIBox::checkMouse(sf::Vector2i mouseCoords)
{
    return (appearance.getGlobalBounds().contains(float(mouseCoords.x), float(mouseCoords.y)));
}

// OBJECT CLASS
Object::Object(const Shape _shape, const int _layer, const float _resti, const float _mass, const float _statF, const float _dynF, sf::Vector2f _pos, const float _size)
{
    shapeType = _shape;
    layer = _layer;

    circleObject.center = _pos + sf::Vector2f(_size, _size);
    circleObject.radius = _size;

    rectangleObject.minCoords = _pos;
    rectangleObject.maxCoords = _pos + sf::Vector2f(2 * _size, 2 * _size);

    circleObject.shape = sf::CircleShape(_size);
    circleObject.shape.setFillColor(sf::Color(100, 250, 100));
    circleObject.shape.setPosition(_pos);

    beginForceArrow = Arrow(circleObject.center, circleObject.center, 2.5f, sf::Color(200, 0, 0));
    beginForce = sf::Vector2f(0.f, 0.f);

    forceArrow = Arrow(circleObject.center, circleObject.center, 2.5f, sf::Color(0, 0, 200));
    force = sf::Vector2f(0.f, 0.f);
    velocity = sf::Vector2f(0.f, 0.f);
    acc = sf::Vector2f(0.f, 0.f);

    restitution = _resti;
    mass = _mass;
    staticFriction = _statF;
    dynamicFriction = _dynF;

    if (mass == 0.f)
    {
        inv_mass = 0.f;
    }

    else
    {
        inv_mass = 1 / mass;
    }

    pos = _pos;
    prevPos = pos;

    empty = false;
    clicked = false;
    released = false;

    guiBox = GUIBox(circleShape, _size, 0.f, _resti, _mass, _statF, _pos.x, _pos.y);
}

Object::Object(const Shape _shape, const int _layer, const float _resti, const float _mass, const float _statF, const float _dynF, sf::Vector2f _pos, const float _xSize, const float _ySize)
{
    shapeType = _shape;
    layer = _layer;

    rectangleObject.minCoords = _pos;
    rectangleObject.maxCoords = _pos + sf::Vector2f(_xSize, _ySize);

    rectangleObject.shape = sf::RectangleShape(sf::Vector2f(_xSize, _ySize));
    rectangleObject.shape.setFillColor(sf::Color(100, 250, 100));
    rectangleObject.shape.setPosition(_pos);

    beginForceArrow = Arrow(_pos + sf::Vector2f(_xSize / 2, _ySize / 2), _pos + sf::Vector2f(_xSize / 2, _ySize / 2), 2.f, sf::Color(200, 0, 0));
    beginForce = sf::Vector2f(0.f, 0.f);

    forceArrow = Arrow(_pos + sf::Vector2f(_xSize / 2, _ySize / 2), _pos + sf::Vector2f(_xSize / 2, _ySize / 2), 2.f, sf::Color(0, 0, 200));
    force = sf::Vector2f(0.f, 0.f);
    velocity = sf::Vector2f(0.f, 0.f);
    acc = sf::Vector2f(0.f, 0.f);

    orientation = 0.f;
    torque = 0.f;
    momentInertia = 1.f;

    restitution = _resti;
    mass = _mass;
    staticFriction = _statF;
    dynamicFriction = _dynF;

    if (mass == 0.f)
    {
        inv_mass = 0.f;
    }

    else
    {
        inv_mass = 1 / mass;
    }

    pos = _pos;
    prevPos = pos;

    empty = false;
    clicked = false;
    released = false;

    guiBox = GUIBox(rectangleShape, _xSize, _ySize, _resti, _mass, _statF, _pos.x, _pos.y);
}

Object::Object()
{
    empty = true;
    clicked = false;
}

void Object::applyForce(float dt, sf::Vector2f gravity, bool launchPlay)
{
    acc = (force + mass * gravity) * inv_mass;
    if (launchPlay)
    {
        acc += (beginForce * 100.f) * inv_mass;
    }

    velocity += acc * dt;
    // angularVel += torque * (1.f / momentInertia) * dt;

    prevPos = pos;
    pos += velocity * dt;
    // orientation += angularVel * dt;

    if (shapeType == circleShape)
    {
        circleObject.center = pos + sf::Vector2f(circleObject.radius, circleObject.radius);

        rectangleObject.minCoords = pos;
        rectangleObject.maxCoords = pos + sf::Vector2f(2 * circleObject.radius, 2 * circleObject.radius);
    }

    else
    {
        rectangleObject.minCoords = pos;
        rectangleObject.maxCoords = pos + sf::Vector2f(rectangleObject.shape.getSize().x, rectangleObject.shape.getSize().y);
    }
}

// void Object::movePause(sf::Vector2f newPos, std::vector<Manifold> &tempManifold) {
//     if (shapeType == rectangleShape) {
//
//     }
//
//     else {
//         pos = newPos;
//         circleObject.shape.setPosition(pos);
//
//         circleObject.center = newPos + sf::Vector2f(circleObject.radius, circleObject.radius);
//         rectangleObject.minCoords = newPos;
//         rectangleObject.maxCoords = pos + sf::Vector2f(2 * circleObject.radius, 2 * circleObject.radius);
//
//         circleObject.shape.setFillColor(sf::Color(100, 250, 100));
//
//         for (unsigned int i = 0; i < tempManifold.size(); i++) {
//             if (tempManifold[i].A->shapeType == rectangleShape) {
//                 if (rectangleCircleCollision(&tempManifold[i])) {
//                     circleObject.shape.setFillColor(sf::Color(200, 250, 200));
//                 }
//             }
//
//             else {
//                 if (circleCollision(&tempManifold[i])) {
//                     circleObject.shape.setFillColor(sf::Color(200, 250, 200));
//                 }
//             }
//         }
//     }
//
//     //reposition force arrow
//     beginForceArrow.startPoint = pos + (rectangleObject.maxCoords - rectangleObject.minCoords) * 0.5f;
//     beginForceArrow.endPoint = beginForceArrow.startPoint + beginForceArrow.vector;
//
//     //change values of textboxes
//     guiBox.textBoxes[0].finalString = toStringFormat(pos.x);
//     guiBox.textBoxes[0].textField.setString(guiBox.textBoxes[0].finalString);
//
//     guiBox.textBoxes[1].finalString = toStringFormat(pos.y);
//     guiBox.textBoxes[1].textField.setString(guiBox.textBoxes[1].finalString);
//
// }

bool Object::checkMouse(sf::Vector2i mouseCoords)
{
    return (contain(rectangleObject.minCoords.x, rectangleObject.maxCoords.x, mouseCoords.x) &&
            contain(rectangleObject.minCoords.y, rectangleObject.maxCoords.y, mouseCoords.y));
}

void Object::reloadGUIBox()
{
    if (shapeType == rectangleShape)
    {
        guiBox = GUIBox(shapeType, rectangleObject.shape.getSize().x, rectangleObject.shape.getSize().y, restitution, mass, staticFriction, pos.x, pos.y);
    }

    else
    {
        guiBox = GUIBox(shapeType, circleObject.radius, 0.f, restitution, mass, staticFriction, pos.x, pos.y);
    }
}

void Object::reinitTextBoxes()
{
    rectangleObject.shape.setFillColor(sf::Color(100, 250, 100));
    circleObject.shape.setFillColor(sf::Color(100, 250, 100));

    for (unsigned int i = 0; i < guiBox.textBoxes.size(); i++)
    {
        guiBox.textBoxes[i].released = false;

        guiBox.textBoxes[i].tempString = "";
        guiBox.textBoxes[i].textField.setString(guiBox.textBoxes[i].finalString);
        changeParameter(guiBox.textBoxes[i].paramNumber, validValue(guiBox.textBoxes[i].finalString, guiBox.textBoxes[i].maxValue, guiBox.textBoxes[i].minValue));

        guiBox.textBoxes[i].cursor = 0;
        guiBox.textBoxes[i].cursorAnimTimer = 0;
    }
}

void Object::changeParameter(int paramNumber, float paramValue)
{
    switch (paramNumber)
    {
        // x size for rectangle shape
    case (0):
        rectangleObject.shape.setSize(sf::Vector2f(paramValue, rectangleObject.shape.getSize().y));
        rectangleObject.maxCoords = rectangleObject.minCoords + sf::Vector2f(paramValue, rectangleObject.shape.getSize().y);

        break;

        // y size for rectangle shape
    case (1):
        rectangleObject.shape.setSize(sf::Vector2f(rectangleObject.shape.getSize().x, paramValue));
        rectangleObject.maxCoords = rectangleObject.minCoords + sf::Vector2f(rectangleObject.shape.getSize().x, paramValue);

        break;

        // radius size for circle shape
    case (2):
        circleObject.shape.setRadius(paramValue);
        circleObject.radius = paramValue;
        circleObject.center = pos + sf::Vector2f(circleObject.radius, circleObject.radius);

        rectangleObject.maxCoords = rectangleObject.minCoords + sf::Vector2f(2 * paramValue, 2 * paramValue);

        break;

        // restitution
    case (3):
        restitution = paramValue;
        break;

        // mass
    case (4):
        mass = paramValue;
        if (paramValue == 0.f)
        {
            inv_mass = 0.f;
        }

        break;

        // friction
    case (5):
        staticFriction = paramValue;

        break;

        // x pos
    case (6):
        pos = sf::Vector2f(paramValue, pos.y);

        if (shapeType == rectangleShape)
        {
            rectangleObject.minCoords = pos;
            rectangleObject.maxCoords = pos + sf::Vector2f(rectangleObject.shape.getSize().x, rectangleObject.shape.getSize().y);
        }

        else
        {
            circleObject.center = pos + sf::Vector2f(circleObject.radius, circleObject.radius);

            rectangleObject.minCoords = pos;
            rectangleObject.maxCoords = pos + sf::Vector2f(2 * circleObject.radius, 2 * circleObject.radius);
        }

        beginForceArrow.startPoint = pos + (rectangleObject.maxCoords - rectangleObject.minCoords) * 0.5f;
        beginForceArrow.setEndPoint(beginForceArrow.startPoint + beginForceArrow.vector);

        forceArrow.startPoint = pos + (rectangleObject.maxCoords - rectangleObject.minCoords) * 0.5f;
        forceArrow.setEndPoint(forceArrow.startPoint + forceArrow.vector);

        break;

        // y position
    case (7):
        pos = sf::Vector2f(pos.x, paramValue);

        if (shapeType == rectangleShape)
        {
            rectangleObject.minCoords = pos;
            rectangleObject.maxCoords = pos + sf::Vector2f(rectangleObject.shape.getSize().x, rectangleObject.shape.getSize().y);
        }

        else
        {
            circleObject.center = pos + sf::Vector2f(circleObject.radius, circleObject.radius);

            rectangleObject.minCoords = pos;
            rectangleObject.maxCoords = pos + sf::Vector2f(2 * circleObject.radius, 2 * circleObject.radius);
        }

        beginForceArrow.startPoint = pos + (rectangleObject.maxCoords - rectangleObject.minCoords) * 0.5f;
        beginForceArrow.setEndPoint(beginForceArrow.startPoint + beginForceArrow.vector);

        forceArrow.startPoint = pos + (rectangleObject.maxCoords - rectangleObject.minCoords) * 0.5f;
        forceArrow.setEndPoint(forceArrow.startPoint + forceArrow.vector);

        break;

        // x beginForce
    case (8):
        beginForce = sf::Vector2f(paramValue, beginForce.y);
        beginForceArrow.setEndPoint(beginForceArrow.startPoint + sf::Vector2f(paramValue, beginForceArrow.vector.y));

        break;

        // y beginForce
    case (9):
        beginForce = sf::Vector2f(beginForce.x, paramValue);
        beginForceArrow.setEndPoint(beginForceArrow.startPoint + sf::Vector2f(beginForceArrow.vector.x, paramValue));
        break;
    }
}