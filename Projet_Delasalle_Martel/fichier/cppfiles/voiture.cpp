#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "voiture.hpp"

#define PI 3.14159265358979323846

using namespace std;
using namespace chrono_literals; // Permet de faire des op�ration de temps avec s, min, h, ...

float switch_posX(const Spawn_area spawn) {

	float X = 0.f;
	switch (spawn) { // Change la valeur de la position x du sprite en fonction de l'endroit o� va appara�tre la voiture
	case Spawn_area::UP: X = 426.f; break;
	case Spawn_area::DOWN: X = 448.f; break;
	case Spawn_area::LEFT: X = -12.f; break;
	case Spawn_area::RIGHT: X = 889.f; break;
	default: X = 254.f; cerr << "Erreur : La creation de la voiture n'a pas pu se faire correctement\n";
	}
	return X;

}

float switch_posY(const Spawn_area spawn) {

	float Y = 0.f;
	switch (spawn) { // Change la valeur de la position y du sprite en fonction de l'endroit o� va appara�tre la voiture
	case Spawn_area::UP: Y = -12.f; break;
	case Spawn_area::DOWN: Y = 681.f; break;
	case Spawn_area::LEFT: Y = 345.f; break;
	case Spawn_area::RIGHT: Y = 323.f; break;
	default: Y = 254.f; cerr << "Erreur : La creation de la voiture n'a pas pu se faire correctement\n";
	}
	return Y;

}

float switch_angle(const Spawn_area spawn) {

	float angle = 0.f;
	switch (spawn) { // Change la valeur de l'angle du sprite en fonction de l'endroit o� va appara�tre la voiture
	case Spawn_area::UP: angle = 180.f; break;
	case Spawn_area::DOWN: angle = 0.f; break;
	case Spawn_area::LEFT: angle = 90.f; break;
	case Spawn_area::RIGHT: angle = -90.f; break;
	default: angle = 0.f; cerr << "Erreur : La creation de la voiture n'a pas pu se faire correctement\n";
	}
	return angle;

}

//float* switch_init_pos(const Spawn_area spawn) {
//
//	float tabTemp[2] = { 0.f, 0.f }; // Premier �l�ment est la position x du cercle de collision, le deuxi�me est la position y
//	switch (spawn) { // Change la valeur de x et y du cercle de collision en fonction de l'endroit o� va appara�tre la voiture
//	case Spawn_area::UP: tabTemp[0] = 180.f; tabTemp[1] = 180.f; break;
//	case Spawn_area::DOWN: tabTemp[0] = 180.f; tabTemp[1] = 180.f; break;
//	case Spawn_area::LEFT: tabTemp[0] = 180.f; tabTemp[1] = 180.f; break;
//	case Spawn_area::RIGHT: tabTemp[0] = 180.f; tabTemp[1] = 180.f; break;
//	default: tabTemp[0] = 180.f; tabTemp[1] = 180.f; cerr << "Erreur : La creation de la voiture n'a pas pu se faire correctement\n";
//	}
//	return tabTemp;
//
//}

Voiture::Voiture(const float speed, const sf::Texture& imageVoiture, const Spawn_area spawn, const Turning turning)
	: spawn_(spawn), turning_(turning), speed_(speed), imageVoiture_(ref(imageVoiture)) {

	spriteVoiture_.setTexture(imageVoiture);
	if (spriteVoiture_.getTexture() == nullptr) {
		cerr << "Erreur : Voiture sans texture\n";
	}
	spriteVoiture_.setOrigin(92.f, 160.f);
	spriteVoiture_.setScale(0.1f, 0.1f);

	posX_ = switch_posX(spawn);
	posY_ = switch_posY(spawn);
	angle_ = switch_angle(spawn);
	spriteVoiture_.setPosition(posX_, posY_);
	spriteVoiture_.setRotation(angle_);

	/*float circleRadius = 20.f;
	circleCollision_.setRadius(circleRadius);
	circleCollision_.setOrigin(circleRadius / 2.f, circleRadius / 2.f);
	posX = const * cos(angle_); posY = const * sin(angle_) */
}

void Voiture::Respawn(const Spawn_area spawn, const Turning turning) {

	turning_ = turning;
	spawn_ = spawn;

	posX_ = switch_posX(spawn);
	posY_ = switch_posY(spawn);
	angle_ = switch_angle(spawn);
	spriteVoiture_.setPosition(posX_, posY_);
	spriteVoiture_.setRotation(angle_);

}

float Voiture::getX() {
	return posX_;
}

float Voiture::getY() {
	return posY_;
}

void Voiture::move() {


	posX_ += static_cast<float>(cos((angle_ - 90) * PI / 180.0) * speed_);
	posY_ += static_cast<float>(sin((angle_ - 90) * PI / 180.0) * speed_);
	spriteVoiture_.setPosition(posX_, posY_);

}

void Voiture::turn() {

	if (turning_ != Turning::NO_TURN) {
		switch (spawn_) {
		case Spawn_area::UP: // original angle : 180
			if (this->getX() <= 530.f && this->getY() >= 317.f && turning_ == Turning::TURN_LEFT) {
				angle_ -= 2.f;
				if (angle_ <= 90.f) {
					angle_ = 90.f;
				}
			}
			else if (this->getX() >= 345.f && this->getY() >= 295.f && turning_ == Turning::TURN_RIGHT) {
				angle_ += 2.f;
				if (angle_ >= 270.f) {
					angle_ = 270.f;
				}
			}
			spriteVoiture_.setRotation(angle_);
			break;
		case Spawn_area::DOWN: // original angle : 0
			if (this->getX() >= 345.f && this->getY() <= 351.f && turning_ == Turning::TURN_LEFT) {
				angle_ -= 2.f;
				if (angle_ < -90.f) {
					angle_ = -90.f;
				}
			}
			else if (this->getX() <= 530.f && this->getY() <= 373.f && turning_ == Turning::TURN_RIGHT) {
				angle_ += 2.f;
				if (angle_ >= 90.f) {
					angle_ = 90.f;
				}
			}
			spriteVoiture_.setRotation(angle_);
			break;
		case Spawn_area::LEFT: // original angle : 90
			if (this->getX() >= 420.f && this->getY() >= 255.f && turning_ == Turning::TURN_LEFT) {
				angle_ -= 2.f;
				if (angle_ <= 0.f) {
					angle_ = 0.f;
				}
			}
			else if (this->getX() >= 398.f && this->getY() <= 420.f && turning_ == Turning::TURN_RIGHT) {
				angle_ += 2.f;
				if (angle_ >= 180.f) {
					angle_ = 180.f;
				}
			}
			spriteVoiture_.setRotation(angle_);
			break;
		case Spawn_area::RIGHT: // original angle : -90
			if (this->getX() <= 454.f && this->getY() <= 420.f && turning_ == Turning::TURN_LEFT) {
				angle_ -= 2.f;
				if (angle_ <= -180.f) {
					angle_ = -180.f;
				}
			}
			else if (this->getX() <= 476.f && this->getY() >= 255.f && turning_ == Turning::TURN_RIGHT) {
				angle_ += 2.f;
				if (angle_ >= 0.f) {
					angle_ = 0.f;
				}
			}
			spriteVoiture_.setRotation(angle_);
			break;
		}
	}

}

//void Voiture::stop() {
//
//
//
//}
