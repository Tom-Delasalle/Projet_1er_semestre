#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "voiture.hpp"

#define PI 3.14159265358979323846

using namespace std;

//float* switch_init_pos(const Spawn_area spawn) {
//
//	float tabTemp[2] = { 0.f, 0.f }; // Premier élément est la position x du cercle de collision, le deuxième est la position y
//	switch (spawn) { // Change la valeur de x et y du cercle de collision en fonction de l'endroit où va apparaître la voiture
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

	moving_ = Moving::CAR;

	spriteVoiture_.setTexture(imageVoiture);
	if (spriteVoiture_.getTexture() == nullptr) {
		cerr << "Erreur : Voiture sans texture\n";
	}
	spriteVoiture_.setOrigin(92.f, 160.f);
	spriteVoiture_.setScale(0.1f, 0.1f);

	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);
	spriteVoiture_.setPosition(posX_, posY_);
	spriteVoiture_.setRotation(angle_);

	radiusCollision_ = 13.f;
	carAndCenterGap_ = 30.f;
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * carAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * carAndCenterGap_;

	//circleTest.setRadius(radiusCollision_);
	//circleTest.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
	//circleTest.setOrigin(circleTest.getRadius(), circleTest.getRadius()); // Définit l'origine du cercle
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre

}

void Voiture::Respawn(const Spawn_area carSpawn, const Turning carTurning) {

	turning_ = carTurning;
	spawn_ = carSpawn;

	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);
	spriteVoiture_.setPosition(posX_, posY_);
	spriteVoiture_.setRotation(angle_);
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * carAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * carAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la 

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
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * carAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * carAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre
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

// Vérifie que l'origine d'un véhicule n'est pas à l'intérieur du cercle de collision
bool Voiture::isNotClose(const Moving moving, const float otherPosX, const float otherPosY) {

	switch (moving) {
	case(Moving::CAR):
		if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
			return false;
		}
		else {
			return true;
		}
		break;
	case(Moving::BUS):
		if (angle_ != 0.f && angle_ != 90.f && angle_ != 180.f && angle_ != -90.f) {
			if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_ + 20.f, 2.f)) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
				return false;
			}
			else {
				return true;
			}
		}
		break;
	case(Moving::BIKE):
		if (angle_ != 0.f && angle_ != 90.f && angle_ != 180.f && angle_ != -90.f) {
			if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_ + 10.f, 2.f)) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
				return false;
			}
			else {
				return true;
			}
		}
		break;
	case(Moving::ON_FOOT):
		if (angle_ != 0.f && angle_ != 90.f && angle_ != 180.f && angle_ != -90.f) {
			if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_ + 15.f, 2.f)) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
				return false;
			}
			else {
				return true;
			}
		}
		break;
	default:
		return true;
	}

}

//void Voiture::stop() {
//
//
//
//}
