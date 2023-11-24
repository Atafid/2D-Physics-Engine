#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "physics.h"

void positionalCorrection(Manifold* m) {
    const float percent = 0.2f;
    const float slop = 0.01f;

    Object* A = m->A;
    Object* B = m->B;

    sf::Vector2f correction = fmaxf(m->penetration - slop, 0.f) / (A->inv_mass + B->inv_mass) * percent * m->normal;

    A->pos -= A->inv_mass * correction;
    B->pos += B->inv_mass * correction;
}

void resolveCollision(Manifold* m) {
    //Impulse part
    Object* A = m->A;
    Object* B = m->B;

    sf::Vector2f relVelo = B->velocity - A->velocity;
    float velNormal = dotProduct(relVelo, m->normal);

    if (velNormal > 0) {
        return;
    }

    float e = fminf(A->restitution, B->restitution);
    float j = (-(1 + e) * velNormal) / (A->inv_mass + B->inv_mass);

    sf::Vector2f impulse = j * m->normal;
    A->velocity -= A->inv_mass * impulse;
    B->velocity += B->inv_mass * impulse;

    positionalCorrection(m);

    //Friction part
    relVelo = B->velocity - A->velocity;
    sf::Vector2f tangentVect = relVelo - dotProduct(relVelo, m->normal) * m->normal;
    normalize(tangentVect);

    float jt = -dotProduct(relVelo, tangentVect);
    jt /= A->inv_mass + B->inv_mass;

    float mu = sqrtf(A->staticFriction * A->staticFriction + B->staticFriction * B->staticFriction);
    sf::Vector2f frictionImpulse;

    if (abs(jt) < mu * j) {
        frictionImpulse = jt * tangentVect;
    }

    else {
        frictionImpulse = -j * sqrtf(A->dynamicFriction*A->dynamicFriction+B->dynamicFriction*B->dynamicFriction) * tangentVect;
    }

    A->velocity -= A->inv_mass * frictionImpulse;
    B->velocity += B->inv_mass * frictionImpulse;
}

bool circleCollision(Manifold* m) {
    Object* c1 = m->A;
    Object* c2 = m->B;

    circle* c1Shape = &(c1->circleObject);
    circle* c2Shape = &(c2->circleObject);

    float radiusLength = c1Shape->radius + c2Shape->radius;
    float squareRadius = powf(radiusLength, 2);

    if (squareRadius < powf((c1Shape->center.x - c2Shape->center.x), 2) + powf((c1Shape->center.y - c2Shape->center.y), 2)) {
        return(false);
    }

    sf::Vector2f n = c2Shape->center - c1Shape->center;
    float d = sqrtf(powf(n.x, 2) + powf(n.y, 2));

    if (d != 0) {
        m->penetration = radiusLength - d;
        m->normal = n / d;

        return(true);
    }

    else {
        m->penetration = radiusLength;
        m->normal = sf::Vector2f(1.0f, 0.f);

        return(true);
    }
}

bool rectangleCollision(Manifold* m) {
    Object* r1 = m->A;
    Object* r2 = m->B;

    AABB* r1Shape = &(r1->rectangleObject);
    AABB* r2Shape = &(r2->rectangleObject);

    float r1HalfX = (r1Shape->maxCoords.x - r1Shape->minCoords.x) / 2.f;
    float r2HalfX = (r2Shape->maxCoords.x - r2Shape->minCoords.x) / 2.f;

    sf::Vector2f relPos = r2->pos - r1->pos;

    float xOverleap = r1HalfX + r2HalfX - abs(relPos.x + r2HalfX - r1HalfX);

    if (xOverleap > 0) {
        float r1HalfY = (r1Shape->maxCoords.y - r1Shape->minCoords.y) / 2.f;
        float r2HalfY = (r2Shape->maxCoords.y - r2Shape->minCoords.y) / 2.f;

        float yOverleap = r1HalfY + r2HalfY - abs(relPos.y + r2HalfY - r1HalfY);

        if (yOverleap > 0) {
            if (xOverleap < yOverleap) {
                m->penetration = xOverleap;

                if (relPos.x > 0) {
                    m->normal = sf::Vector2f(1.f, 0.f);
                }

                else {
                    m->normal = sf::Vector2f(-1.f, 0.f);
                }

                return(true);
            }

            else {
                m->penetration = yOverleap;

                if (relPos.y > 0) {
                    m->normal = sf::Vector2f(0.f, 1.f);
                }

                else {
                    m->normal = sf::Vector2f(0.f, -1.f);
                }

                return(true);
            }
        }
    }

    return(false);
}

bool rectangleCircleCollision(Manifold* m) {
    Object* r = m->A;
    Object* c = m->B;

    AABB* rShape = &(r->rectangleObject);
    circle* cShape = &(c->circleObject);

    float rad = cShape->radius;

    float xHalf = (rShape->maxCoords.x - rShape->minCoords.x) / 2.f;
    float yHalf = (rShape->maxCoords.y - rShape->minCoords.y) / 2.f;

    sf::Vector2f relPos = (c->pos + sf::Vector2f(rad, rad)) - (r->pos + sf::Vector2f(xHalf, yHalf));
    sf::Vector2f closest = relPos;

    closest.x = clamp(-xHalf, xHalf, closest.x);
    closest.y = clamp(-yHalf, yHalf, closest.y);

    bool isInside = false;

    if (closest == relPos) {
        isInside = true;

        if (abs(closest.x) > abs(closest.y)) {
            closest.x = copysignf(xHalf, closest.x);
        }

        else {
            closest.y = copysignf(yHalf, closest.y);
        }
    }

    sf::Vector2f n = relPos - closest;
    float d = powf(n.x, 2.f) + powf(n.y, 2.f);

    if (d > rad * rad && !isInside) {
        return(false);
    }

    d = sqrtf(d);

    if (isInside) {
        m->normal = -n / d;
    }

    else {
        m->normal = n / d;
    }

    m->penetration = rad - d;

    return(true);
}

bool circleRectangleCollision(Manifold* m) {
    Object* c = m->A;
    Object* r = m->B;

    circle* cShape = &(c->circleObject);
    AABB* rShape = &(r->rectangleObject);

    float rad = cShape->radius;

    float xHalf = (rShape->maxCoords.x - rShape->minCoords.x) / 2.f;
    float yHalf = (rShape->maxCoords.y - rShape->minCoords.y) / 2.f;

    sf::Vector2f relPos = (c->pos + sf::Vector2f(rad, rad)) - (r->pos + sf::Vector2f(xHalf, yHalf));
    sf::Vector2f closest = relPos;

    closest.x = clamp(-xHalf, xHalf, closest.x);
    closest.y = clamp(-yHalf, yHalf, closest.y);

    bool isInside = false;

    if (closest == relPos) {
        isInside = true;

        if (abs(relPos.x) > abs(relPos.y)) {
            closest.x = copysignf(xHalf, closest.x);
        }

        else {
            closest.y = copysignf(yHalf, closest.y);
        }
    }

    sf::Vector2f n = relPos - closest;
    float d = powf(n.x, 2.f) + powf(n.y, 2.f);

    if (d > rad * rad && !isInside) {
        return(false);
    }

    d = sqrtf(d);

    if (isInside) {
        m->normal = -n / d;
    }

    else {
        m->normal = n / d;
    }

    m->penetration = rad - d;

    m->normal = -m->normal;

    return(true);
}