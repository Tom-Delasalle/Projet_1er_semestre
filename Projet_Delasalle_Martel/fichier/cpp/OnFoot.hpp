
#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "voiture.hpp"


class OnFoot {
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
	float pedestrianAndCenterGap_;
public:
	sf::Texture imagePieton_;
	sf::Sprite spritePieton_;
	//sf::CircleShape circleTest;
	// Constructor
	OnFoot(const float speed, const sf::Texture& imagePieton, const Spawn_area spawn);
	// Functions
	void Respawn(const Spawn_area& spawn);
	float getX();
	float getY();
	void move();
	//void stop();
	bool isNotClose(const Moving moving, const float otherPosX, const float otherPosY);
};
