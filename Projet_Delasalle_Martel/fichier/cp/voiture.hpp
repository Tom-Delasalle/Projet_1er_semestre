#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tricolore.hpp"

enum class Spawn_area {
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
};

enum class Turning {
	TURN_LEFT = 1,
	TURN_RIGHT = 2,
	NO_TURN = 3
};

class Voiture {
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
	float carAndCenterGap_;
public:
	sf::Texture imageVoiture_;
	sf::Sprite spriteVoiture_;
	//f::CircleShape circleTest;
	// Constructor
	Voiture(const float speed, const sf::Texture& imageVoiture, const Spawn_area spawn, const Turning turning);
	// Functions
	void Respawn(const Spawn_area spawn, const Turning turning);
	float getX();
	float getY();
	void move();
	//void stop();
	void turn();
	bool isNotClose(const float otherPosX, const float otherPosY);
};