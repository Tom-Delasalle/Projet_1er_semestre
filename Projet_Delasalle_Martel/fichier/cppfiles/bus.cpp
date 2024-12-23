#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "bus.hpp"

#define PI 3.14159265358979323846

using namespace std;

Bus::Bus(const float speed, const sf::Texture& imageBus, const Spawn_area spawn)
	: spawn_(spawn), speed_(speed), imageBus_(ref(imageBus)) {

	moving_ = Moving::BUS;

	spriteBus_.setTexture(imageBus);
	if (spriteBus_.getTexture() == nullptr) {
		cerr << "Erreur : Bus sans texture\n";
	}
	spriteBus_.setOrigin(133.f, 329.f);
	spriteBus_.setScale(0.07f, 0.07f);

	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);
	spriteBus_.setPosition(posX_, posY_);
	spriteBus_.setRotation(angle_);

	radiusCollision_ = 15.f;
	busAndCenterGap_ = 40.f;
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * busAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * busAndCenterGap_;

	//circleTest.setRadius(radiusCollision_);
	//circleTest.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
	//circleTest.setOrigin(circleTest.getRadius(), circleTest.getRadius()); // Définit l'origine du cercle
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre

}

void Bus::Respawn(const Spawn_area spawn) {

	spawn_ = spawn;
	posX_ = switch_posX(moving_, spawn_);
	posY_ = switch_posY(moving_, spawn_);
	angle_ = switch_angle(spawn_);
	spriteBus_.setPosition(posX_, posY_);
	spriteBus_.setRotation(angle_);
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * busAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * busAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la 

}

float Bus::getX() {
	return posX_;
}

float Bus::getY() {
	return posY_;
}

void Bus::move() {

	posX_ += static_cast<float>(cos((angle_ - 90) * PI / 180.0) * speed_);
	posY_ += static_cast<float>(sin((angle_ - 90) * PI / 180.0) * speed_);
	spriteBus_.setPosition(posX_, posY_);
	centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * busAndCenterGap_;
	centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * busAndCenterGap_;
	//circleTest.setPosition(centerCollisionX_, centerCollisionY_); // Définit la position du cercle dans la fenêtre
}

// Vérifie que l'origine d'un véhicule n'est pas à l'intérieur du cercle de collision
bool Bus::isNotClose(const float otherPosX, const float otherPosY) {

	if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
		return false;
	}
	else {
		return true;
	}

}

//void Bus::stop() {
//
//
//
//}