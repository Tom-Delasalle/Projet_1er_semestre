#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

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

enum class Moving {
	CAR = 1,
	BUS = 2,
	BIKE = 3,
	ON_FOOT = 4
};

class Voiture {
private:
	Moving moving_;
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
	//sf::CircleShape circleTest;
	// Constructor
	Voiture(const float speed, const sf::Texture& imageVoiture, const Spawn_area spawn, const Turning turning);
	// Functions
	void Respawn(const Spawn_area spawn, const Turning turning);
	float getX();
	float getY();
	void move();
	//void stop();
	void turn();
	bool isNotClose(const Moving moving, const float otherPosX, const float otherPosY);
};

inline float switch_posX(const Moving moving, const Spawn_area spawn) {

	float X = 0.f;
	switch (moving) {
	case (Moving::CAR):
		switch (spawn) { // Change la valeur de la position x du sprite en fonction de l'endroit où va apparaître la voiture
		case Spawn_area::UP: X = 426.f; break;
		case Spawn_area::DOWN: X = 448.f; break;
		case Spawn_area::LEFT: X = -17.f; break;
		case Spawn_area::RIGHT: X = 894.f; break;
		default: X = 254.f; cerr << "Erreur : La creation de la voiture n'a pas pu se faire correctement\n";
		}
		break;
	case(Moving::BUS):
		switch (spawn) {
		case Spawn_area::LEFT: X = -17.f; break;
		case Spawn_area::RIGHT: X = 894.f; break;
		default: X = 254.f; cerr << "Erreur : La creation du bus n'a pas pu se faire correctement\n";
		}
		break;
	case(Moving::BIKE): X = 400.f; break;
	case(Moving::ON_FOOT):
		switch (spawn) { // Change la valeur de la position x du sprite en fonction de l'endroit où va apparaître le piéton
		case Spawn_area::UP: X = 380.f; break;
		case Spawn_area::DOWN: X = 495.f; break;
		case Spawn_area::LEFT: X = 3.f; break;
		case Spawn_area::RIGHT: X = 871.f; break;
		default: X = 254.f; cerr << "Erreur : La creation dU piéton n'a pas pu se faire correctement\n";
		}
		break;
	default: cerr << "Erreur : enum class moving fonction switch_posX\n";
	}
	return X;

}

inline float switch_posY(const Moving moving, const Spawn_area spawn) {

	float Y = 0.f;
	switch (moving) {
	case(Moving::CAR):
		switch (spawn) { // Change la valeur de la position y du sprite en fonction de l'endroit où va apparaître la voiture
		case Spawn_area::UP: Y = -17.f; break;
		case Spawn_area::DOWN: Y = 676.f; break;
		case Spawn_area::LEFT: Y = 345.f; break;
		case Spawn_area::RIGHT: Y = 323.f; break;
		default: Y = 254.f; cerr << "Erreur : La creation de la voiture n'a pas pu se faire correctement\n";
		}
		break;
	case(Moving::BUS):
		switch (spawn) {
		case Spawn_area::LEFT: Y = 366.f; break;
		case Spawn_area::RIGHT: Y = 306.f; break;
		default: Y = 254.f; cerr << "Erreur : La creation du bus n'a pas pu se faire correctement\n";
		}
		break;
	case(Moving::BIKE): Y = 400.f; break;
	case(Moving::ON_FOOT):
		switch (spawn) { // Change la valeur de la position y du sprite en fonction de l'endroit où va apparaître la voiture
		case Spawn_area::UP: Y = 3.f; break;
		case Spawn_area::DOWN: Y = 659.f; break;
		case Spawn_area::LEFT: Y = 390.f; break;
		case Spawn_area::RIGHT: Y = 280.f; break;
		default: Y = 254.f; cerr << "Erreur : La creation du piéton n'a pas pu se faire correctement\n";
		}
		break;
	default: cerr << "Erreur : enum class moving fonction switch_posY\n";
	}
	return Y;

}

inline float switch_angle(const Moving moving, const Spawn_area spawn) {

	float angle = 0.f;
	switch (moving) {
	case(Moving::CAR):
		switch (spawn) { // Change la valeur de l'angle du sprite en fonction de l'endroit où va apparaître la voiture
		case Spawn_area::UP: angle = 180.f; break;
		case Spawn_area::DOWN: angle = 0.f; break;
		case Spawn_area::LEFT: angle = 90.f; break;
		case Spawn_area::RIGHT: angle = -90.f; break;
		default: angle = 0.f; cerr << "Erreur : La creation de la voiture n'a pas pu se faire correctement\n";
		}
		break;
	case(Moving::BUS):
		switch (spawn) {
		case Spawn_area::LEFT: angle = 90.f; break;
		case Spawn_area::RIGHT: angle = -90.f; break;
		default: angle = 254.f; cerr << "Erreur : La creation du bus n'a pas pu se faire correctement\n";
		}
		break;
	case(Moving::BIKE): angle = 0.f; break;
	case(Moving::ON_FOOT):
		switch (spawn) { // Change la valeur de l'angle du sprite en fonction de l'endroit où va apparaître le piéton
		case Spawn_area::UP: angle = 180.f; break;
		case Spawn_area::DOWN: angle = 0.f; break;
		case Spawn_area::LEFT: angle = 90.f; break;
		case Spawn_area::RIGHT: angle = -90.f; break;
		default: angle = 0.f; cerr << "Erreur : La creation du pieton n'a pas pu se faire correctement\n";
		}
		break;
	default: cerr << "Erreur : enum class moving fonction switch_angle\n";
	}
	return angle;

}