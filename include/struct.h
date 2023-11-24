#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#define PI 3.141592653589793238462643383279502884197169399

#include "functions.h"
#include "button.h"

extern sf::Texture textures[6];
extern sf::Font fonts[1];

struct AABB
{
    sf::Vector2f minCoords;
    sf::Vector2f maxCoords;

    sf::RectangleShape shape;
};

struct circle
{
    sf::Vector2f center;
    float radius;

    sf::CircleShape shape;
};

enum Shape
{
    circleShape,
    rectangleShape
};

class Arrow
{
public:
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;
    sf::Vector2f vector;

    Arrow(sf::Vector2f _startPoint, sf::Vector2f _endPoint, float _thickness, sf::Color _color);
    Arrow(){};
    ~Arrow(){};

    void setEndPoint(sf::Vector2f _endPoint);
    void draw(sf::RenderTarget &target);

private:
    sf::RectangleShape tail;
    sf::CircleShape triangle;

    float thickness;
    float length;
    float angle;
};

class GUIBox
{
public:
    Shape objectShape;
    sf::RectangleShape appearance;

    std::vector<TextBox> textBoxes;

    GUIBox(Shape _shape, float xsize, float ysize, float resti, float mass, float friction, float xPos, float yPos);
    GUIBox(){};
    ~GUIBox(){};

    void display(sf::RenderWindow &targetWindow, sf::Vector2f coords);
    bool checkMouse(sf::Vector2i mouseCoords);

private:
    TextBox textBox;
};

class Object
{
public:
    Shape shapeType;
    circle circleObject;
    AABB rectangleObject;

    int layer;

    Arrow beginForceArrow;
    sf::Vector2f beginForce;

    Arrow forceArrow;
    sf::Vector2f force;
    sf::Vector2f velocity;
    sf::Vector2f acc;
    float angularVel;
    float torque;

    float restitution;
    float mass;
    float inv_mass;
    float momentInertia;
    float staticFriction;
    float dynamicFriction;

    sf::Vector2f pos;
    sf::Vector2f prevPos;
    float orientation;

    bool empty;
    bool clicked;
    bool released;

    GUIBox guiBox;

    Object(const Shape _shape, const int _layer, const float _resti, const float _mass, const float _statF, const float _dynF, sf::Vector2f _pos, const float _size);
    Object(const Shape _shape, const int _layer, const float _resti, const float _mass, const float _statF, const float _dynF, sf::Vector2f _pos, const float _xSize, const float _ySize);
    Object();
    ~Object(){};

    void applyForce(float dt, sf::Vector2f gravity, bool launchPlay);

    // void movePause(sf::Vector2f newPos, std::vector<Manifold>& tempManifold);
    bool checkMouse(sf::Vector2i mouseCoords);
    void reloadGUIBox();
    void reinitTextBoxes();
    void changeParameter(int paramNumber, float paramValue);
};

struct Manifold
{
    Object *A;
    Object *B;

    float penetration;
    sf::Vector2f normal;
};