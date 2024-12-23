#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "voiture.hpp"

class Bus {
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
	float busAndCenterGap_;
public:
	sf::Texture imageBus_;
	sf::Sprite spriteBus_;
	//sf::CircleShape circleTest;
	// Constructor
	Bus(const float speed, const sf::Texture& imageBus, const Spawn_area spawn);
	// Functions
	void Respawn(const Spawn_area spawn);
	float getX();
	float getY();
	void move();
	//void stop();
	bool isNotClose(const float otherPosX, const float otherPosY);
};