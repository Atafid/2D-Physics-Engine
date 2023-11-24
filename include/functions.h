#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

float dotProduct(sf::Vector2f v1, sf::Vector2f v2);
float len(sf::Vector2f v);
float clamp(float min, float max, float a);
void normalize(sf::Vector2f& v);
bool contain(float min, float max, int a);

bool isValidCharacter(sf::Keyboard::Key code);
float validValue(std::string strValue, float max, float min);
std::string toStringFormat(float val);