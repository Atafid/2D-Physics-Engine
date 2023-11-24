#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "functions.h"
#include "struct.h"

void positionalCorrection(Manifold* m);
void resolveCollision(Manifold* m);

bool circleCollision(Manifold* m);
bool rectangleCollision(Manifold* m);
bool rectangleCircleCollision(Manifold* m);
bool circleRectangleCollision(Manifold* m);
