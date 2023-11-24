#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "functions.h"

float dotProduct(sf::Vector2f v1, sf::Vector2f v2) {
    return(v1.x * v2.x + v1.y * v2.y);
}

float len(sf::Vector2f v) {
    return(sqrtf(v.x * v.x + v.y * v.y));
}

float clamp(float min, float max, float a) {
    if (a < min) {
        return(min);
    }

    if (a > max) {
        return(max);
    }

    return(a);
}

void normalize(sf::Vector2f& v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);

    if (length != 0.f) {
        v.x /= length;
        v.y /= length;
    }
}

bool contain(float min, float max, int a) {
    return(min <= a && max >= a);
}

bool isValidCharacter(sf::Keyboard::Key code) {
    //check if the key pressed is one of the number on pad
    return((code >= 48 && code <= 57) || code == 46 || code == 45);
}

float validValue(std::string strValue, float max, float min) {
    try {
        bool minus = false;

        if (strValue[0] == '-') {
            strValue = strValue.substr(1, strValue.size() - 1);
            minus = true;
        }

        float paramValue = std::stof(strValue);
        paramValue *= pow(-1, (int)minus);

        return(std::max(std::min(paramValue, max), min));
    }

    catch(std::invalid_argument) {
        throw std::invalid_argument("");
    }
}

std::string toStringFormat(float val) {
    std::string res = std::to_string(val);
    bool pointPassed = false;
    int counter = 0;
    int finalSize = 0;

    for (unsigned int i = 0; i < res.size(); i++) {
        if (pointPassed) {
            counter++;
        }

        if (res[i] == '.') {
            pointPassed = true;
        }

        if (counter > 2) {
            break;
        }

        finalSize++;
    }

    if (res[finalSize-1] == '0') {
        finalSize--;

        if (res[finalSize-1] == '0') {
            finalSize--;
        }
    }

    return(res.substr(0, finalSize));
}