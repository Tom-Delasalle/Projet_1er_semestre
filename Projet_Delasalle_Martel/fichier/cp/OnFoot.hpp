
#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "common.hpp"


class OnFoot {
private:
	float posX_;
	float posY_;
	float speed_;
	float angle_;
	Spawn_area spawn_;
	Turning turning_;
	float centerCollisionX_;
	float centerCollisionY_;
	float radiusCollision_;
	float pedestrianAndCenterGap_;
public:
	sf::Texture imagePieton_;
	sf::Sprite spritePieton_;
	// Constructor
	OnFoot(const float speed, const sf::Texture& imagePieton, const Spawn_area spawn, const Turning turning);
	// Functions
	void Respawn(const Spawn_area& spawn, const Turning& turning);
	float getX();
	float getY();
	void move();
	//void stop();
	void turn();
	bool isNotClose(const float otherPosX, const float otherPosY);
};
