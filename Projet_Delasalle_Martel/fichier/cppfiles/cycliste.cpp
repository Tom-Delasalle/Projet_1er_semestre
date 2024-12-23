#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "cycliste.hpp"

#define PI 3.14159265358979323846

using namespace std;

Cycliste::Cycliste(const float speed, const sf::Texture& imageCycliste, const Spawn_area spawn)
	: spawn_(spawn), speed_(speed), imageCycliste_(ref(imageCycliste)) {

	moving_ = Moving::BIKE;

	spriteCycliste_.setTexture(imageCycliste);
	if (spriteCycliste_.getTexture() == nullptr) {
		cerr << "Erreur : Cycliste sans texture\n";
	}
	spriteCycliste_.setOrigin(92.5f, 169.5f);
	spriteCycliste_.setScale(0.08f, 0.08f);

	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);
	spriteCycliste_.setPosition(posX_, posY_);
	spriteCycliste_.setRotation(angle_);

	radiusCollision_ = 15.f;
	bikeAndCenterGap_ = 30.f;
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * bikeAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * bikeAndCenterGap_;

	//circleTest.setRadius(radiusCollision_);
	//circleTest.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
	//circleTest.setOrigin(circleTest.getRadius(), circleTest.getRadius()); // Définit l'origine du cercle
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre

}

void Cycliste::Respawn(const Spawn_area spawn) {

	spawn_ = spawn;
	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);
	spriteCycliste_.setPosition(posX_, posY_);
	spriteCycliste_.setRotation(angle_);
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * bikeAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * bikeAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la 

}

float Cycliste::getX() {
	return posX_;
}

float Cycliste::getY() {
	return posY_;
}

void Cycliste::move() {

	posX_ += static_cast<float>(cos((angle_ - 90) * PI / 180.0) * speed_);
	posY_ += static_cast<float>(sin((angle_ - 90) * PI / 180.0) * speed_);
	spriteCycliste_.setPosition(posX_, posY_);
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * bikeAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * bikeAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre
}

// Vérifie que l'origine d'un véhicule n'est pas à l'intérieur du cercle de collision
bool Cycliste::isNotClose(const float otherPosX, const float otherPosY) {

	if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
		return false;
	}
	else {
		return true;
	}

}

//void Cycliste::stop() {
//
//
//
//}