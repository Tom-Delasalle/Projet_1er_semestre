#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Follower.hpp"


class Leader
{
public:
	Leader(const std::string &filename, const float &x, const float &y, 
	const float &scale = 1.f);

	float getX() const;
	float getY() const;
	float getAngle() const;

	void move();

	void turn();

	void turnLeft();
	void turnRight();
	void speedUp();
	void speedDown();

	void draw(sf::RenderWindow &app);

	void add_followers(Follower &followers);

private:
	float x_, y_;
	float angle_;
	float speed_;
	sf::Texture image_;
	sf::Sprite  sprite_;
	std::vector<Follower> followers;
};
