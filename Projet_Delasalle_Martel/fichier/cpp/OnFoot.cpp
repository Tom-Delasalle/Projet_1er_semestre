#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "OnFoot.hpp"

#define PI 3.14159265358979323846

using namespace std;

OnFoot::OnFoot(const float speed, const sf::Texture& imagePieton, const Spawn_area spawn)
	: spawn_(spawn), speed_(speed), imagePieton_(ref(imagePieton)) {

	moving_ = Moving::ON_FOOT;

	spritePieton_.setTexture(imagePieton);
	if (spritePieton_.getTexture() == nullptr) {
		cerr << "Erreur : Pieton sans texture\n";
	}
	spritePieton_.setOrigin(73.5f, 112.f);
	spritePieton_.setScale(0.1f, 0.1f);

	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);
	spritePieton_.setPosition(posX_, posY_);
	spritePieton_.setRotation(angle_);

	radiusCollision_ = 8.f;
	pedestrianAndCenterGap_ = 40.f;
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;

	//circleTest.setRadius(radiusCollision_);
	//circleTest.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
	//circleTest.setOrigin(circleTest.getRadius(), circleTest.getRadius()); // Définit l'origine du cercle
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre

}



void OnFoot::Respawn(const Spawn_area& spawn) {

	spawn_ = spawn;

	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);

	spritePieton_.setPosition(posX_, posY_);
	spritePieton_.setRotation(angle_);

	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la 

}

float OnFoot::getX() {
	return posX_;
}

float OnFoot::getY() {
	return posY_;
}

void OnFoot::move() {

	posX_ += static_cast<float>(cos((angle_ - 90) * PI / 180.0) * speed_);
	posY_ += static_cast<float>(sin((angle_ - 90) * PI / 180.0) * speed_);
	spritePieton_.setPosition(posX_, posY_);
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre

}

// Vérifie que l'origine d'un piéton n'est pas à l'intérieur du cercle de collision
bool OnFoot::isNotClose(const Moving moving, const float otherPosX, const float otherPosY) {

	switch (moving) {
	case(Moving::CAR):
		if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_ + 14.f, 2.f)) {
			return false;
		}
		else {
			return true;
		}
		break;
	case(Moving::BUS):
		if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_ + 14.f, 2.f)) {
			return false;
		}
		else {
			return true;
		}
		break;
	case(Moving::BIKE):
		if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_ + 14.f, 2.f)) {
			return false;
		}
		else {
			return true;
		}
		break;
	case(Moving::ON_FOOT):
		if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
			return false;
		}
		else {
			return true;
		}
	default:
		return true;
	}

}

//void OnFoot::stop() {
//
//
//
//}
