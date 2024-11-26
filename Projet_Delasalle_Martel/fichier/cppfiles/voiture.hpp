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
	int posX_;
	int posY_;
	float speed_;
	Spawn_area spawn_;
	Turning turning_;
	sf::Texture imageVoiture_;
	sf::Sprite spriteVoiture_;
public:
	// Constructor
	Voiture(const float speed, const Spawn_area& spawn, const Turning& turning);
	// Functions
	int getX();
	int getY();
	void set_speed(const float newSpeed);
	void move();
	void stop();
	void turn();
};