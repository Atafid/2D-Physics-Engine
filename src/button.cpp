#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "button.h"
#include "functions.h"

//BUTTONS
Button::Button(std::string spritesPath[6], sf::Vector2f coords, bool _oneState) {
	mainTexture.loadFromFile(spritesPath[0]);
	mainApp = sf::Sprite(mainTexture);

	mouseTexture.loadFromFile(spritesPath[1]);
	mouseApp = sf::Sprite(mouseTexture);

	clickTexture.loadFromFile(spritesPath[2]);
	clickApp = sf::Sprite(clickTexture);

	releaseTexture.loadFromFile(spritesPath[3]);
	releaseApp = sf::Sprite(releaseTexture);

	mouseReleaseTexture.loadFromFile(spritesPath[4]);
	mouseReleaseApp = sf::Sprite(mouseReleaseTexture);

	clickReleaseTexture.loadFromFile(spritesPath[5]);
	clickReleaseApp = sf::Sprite(clickReleaseTexture);

	app = mainApp;

	app.setPosition(coords);

	mainApp.setPosition(coords);
	mouseApp.setPosition(coords);
	clickApp.setPosition(coords);
	releaseApp.setPosition(coords);
	mouseReleaseApp.setPosition(coords);
	clickReleaseApp.setPosition(coords);

	sprites = { {mainApp, mouseApp, clickApp},
			   {releaseApp, mouseReleaseApp, clickReleaseApp} };

	clicked = false;
	released = false;
	empty = false;

	oneState = _oneState;
}

Button::Button(sf::Texture textures[6], sf::Vector2f coords, bool _oneState) {
	mainApp = sf::Sprite(textures[0]);
	mouseApp = sf::Sprite(textures[1]);
	clickApp = sf::Sprite(textures[2]);

	releaseApp = sf::Sprite(textures[3]);
	mouseReleaseApp = sf::Sprite(textures[4]);
	clickReleaseApp = sf::Sprite(textures[5]);

	app = mainApp;

	app.setPosition(coords);

	mainApp.setPosition(coords);
	mouseApp.setPosition(coords);
	clickApp.setPosition(coords);
	releaseApp.setPosition(coords);
	mouseReleaseApp.setPosition(coords);
	clickReleaseApp.setPosition(coords);

	sprites = { {mainApp, mouseApp, clickApp},
			   {releaseApp, mouseReleaseApp, clickReleaseApp} };

	clicked = false;
	released = false;
	empty = false;

	oneState = _oneState;
}

Button::Button() {
	clicked = false;
	released = false;
	empty = true;
	oneState = false;
}

Button::~Button() {}

bool Button::checkMouse(sf::Vector2i mouseCoords) {
	return(app.getGlobalBounds().contains(float(mouseCoords.x), float(mouseCoords.y)));
}

bool Button::handleApp(sf::Vector2i mouseCoords) {
	if (this->checkMouse(mouseCoords)) {
		if (clicked) {
			app = sprites[released][2];
		}

		else {
			app = sprites[released][1];
		}

		return(true);
	}

	else {
		clicked = false;
		app = sprites[released][0];

		return(false);
	}
}

//TEXTBOX
TextBox::TextBox(sf::Texture textures[6], sf::Vector2f coords, sf::Font fonts[1], std::string label, float initValue, int _paramNumber, float _maxValue, float _minValue) : Button(textures, sf::Vector2f(0.f, 0.f), true) {
	coordsGUIWorld = coords;
	textLabel.setFont(fonts[0]);
	textLabel.setCharacterSize(10);
	textLabel.setFillColor(sf::Color::White);
	textLabel.setString(label);

	textField.setFont(fonts[0]);
	textField.setCharacterSize(17);
	textField.setFillColor(sf::Color::Black);
	textField.setLetterSpacing(1);

	cursorApp.setFont(fonts[0]);
	cursorApp.setCharacterSize(15);
	cursorApp.setFillColor(sf::Color::Black);
	cursorApp.setString("|");

	finalString = toStringFormat(initValue);
	textField.setString(finalString);

	tempString = "";

	cursor = 0;

	paramNumber = _paramNumber;
	maxValue = _maxValue;
	minValue = _minValue;
}

bool TextBox::checkMouse(sf::Vector2f guiBoxCoords, sf::Vector2i mouseCoords) {
	return((guiBoxCoords + coordsGUIWorld).x <= float(mouseCoords.x) &&
		(guiBoxCoords + coordsGUIWorld).x + app.getGlobalBounds().getSize().x >= float(mouseCoords.x) &&
		(guiBoxCoords + coordsGUIWorld).y < float(mouseCoords.y) &&
		(guiBoxCoords + coordsGUIWorld).y + app.getGlobalBounds().getSize().y >= float(mouseCoords.y));
}

bool TextBox::handleApp(sf::Vector2f guiBoxCoords, sf::Vector2i mouseCoords) {
	if (this->checkMouse(guiBoxCoords, mouseCoords)) {
		if (clicked) {
			app = sprites[released][2];
		}

		else {
			app = sprites[released][1];
		}

		return(true);
	}

	else {
		clicked = false;
		app = sprites[released][0];

		return(false);
	}
}