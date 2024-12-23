#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "voiture.hpp"

class Cycliste {
private:
	Moving moving_;
	float posX_;
	float posY_;
	float speed_;
	float angle_;
	Spawn_area spawn_;
	float centerCollisionX_;
	float centerCollisionY_;
	float radiusCollision_;
	float bikeAndCenterGap_;
public:
	sf::Texture imageCycliste_;
	sf::Sprite spriteCycliste_;
	sf::CircleShape circleTest;
	// Constructor
	Cycliste(const float speed, const sf::Texture& imageCycliste, const Spawn_area spawn);
	// Functions
	void Respawn(const Spawn_area spawn);
	float getX();
	float getY();
	void move();
	//void stop();
	bool isNotClose(const float otherPosX, const float otherPosY);
};