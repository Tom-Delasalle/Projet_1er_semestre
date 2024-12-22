#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tricolore.hpp"
#include "common.hpp"



class Voiture {
private:
	float posX_;
	float posY_;
	float speed_;
	float angle_;
	Spawn_area spawn_;
	Turning turning_;
public:
	sf::Texture imageVoiture_;
	sf::Sprite spriteVoiture_;
	sf::CircleShape circleCollision_;
	// Constructor
	Voiture(const float speed, const sf::Texture& imageVoiture, const Spawn_area spawn, const Turning turning);
	// Functions
	void Respawn(const Spawn_area spawn, const Turning turning);
	float getX();
	float getY();
	void move();
	//void stop();
	void turn();
};
