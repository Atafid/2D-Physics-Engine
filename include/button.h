#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

class Button {
public:
	sf::Sprite app;

	std::vector<std::vector<sf::Sprite>> sprites;

	bool clicked; 
	bool released;
	bool empty;

	bool oneState;

	Button(std::string spritesPath[6], sf::Vector2f coords, bool _oneState);
	Button(sf::Texture textures[6], sf::Vector2f coords, bool _oneState);
	Button();
	~Button();

	bool checkMouse(sf::Vector2i mouseCoords);
	bool handleApp(sf::Vector2i mouseCoords);

private:
	sf::Texture mainTexture;
	sf::Texture mouseTexture;
	sf::Texture clickTexture;
	sf::Texture releaseTexture;
	sf::Texture mouseReleaseTexture;
	sf::Texture clickReleaseTexture;

	sf::Sprite mainApp;
	sf::Sprite mouseApp;
	sf::Sprite clickApp;
	sf::Sprite releaseApp;
	sf::Sprite mouseReleaseApp;
	sf::Sprite clickReleaseApp;
};

class TextBox : public Button {
public:
	sf::Vector2f coordsGUIWorld;
	sf::Text textLabel;

	sf::Text cursorApp;
	int cursor;
	int cursorAnimTimer;

	sf::Text textField;
	std::string finalString;
	std::string tempString;

	int paramNumber;
	float maxValue;
	float minValue;

	TextBox(sf::Texture textures[6], sf::Vector2f coords, sf::Font fonts[1], std::string label, float initValue, int _paramNumber, float _maxValue, float _minValue);
	TextBox() : Button() {};
	~TextBox() {};

	bool checkMouse(sf::Vector2f guiBoxCoords, sf::Vector2i mouseCoords);
	bool handleApp(sf::Vector2f guiBoxCoords, sf::Vector2i mouseCoords);
};
