#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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


class OnFoot {
private:
	float posX_;
	float posY_;
	float speed_;
	float angle_;
	Spawn_area spawn_;
	Turning turning_;
public:
	sf::Texture imagePieton_;
	sf::Sprite spritePieton_;
	// Constructor
	OnFoot(const float speed, const sf::Texture& imagePieton, const Spawn_area spawn, const Turning turning);
	// Functions
	void Respawn(const Spawn_area& spawn, const Turning& turning);
	float getX();
	float getY();
	void set_speed(const float newSpeed);
	void move();
	//void stop();
	void turn();
};