#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "voiture.hpp"

# define PI 3.14159265358979323846

using namespace std;
using namespace chrono_literals; // Permet de faire des opération de temps avec s, min, h, ...

Voiture::Voiture(const float speed, const Spawn_area& spawn, const Turning& turning) : spawn_(spawn), turning_(turning), speed_(speed) {

    // Charge l'image de la voiture
    if (!imageVoiture_.loadFromFile("../../../../img/voiture.png")) {
        cerr << "Erreur : Impossible de charger l'image voiture.png\n";
    }
    spriteVoiture_.setTexture(imageVoiture_);
    
    switch (spawn) {
    case Spawn_area::UP:
        posX_ = 455;
        posY_ = 3;
        angle_ = 180.f;
        break;
    case Spawn_area::DOWN:
        posX_ = 425;
        posY_ = 659;
        angle_ = 270.f;
        break;
    case Spawn_area::LEFT:
        posX_ = 3;
        posY_ = 339;
        angle_ = 0.f;
        break;
    case Spawn_area::RIGHT:
        posX_ = 871;
        posY_ = 332;
        angle_ = 90.f;
        break;
    default:
        posX_ = 254;
        posY_ = 254;
        angle_ = 0.f;
        cerr << "Erreur : La position x y des voitures n'a pas pu se faire correctement\n";
    }
    spriteVoiture_.setRotation(angle_);

}

int Voiture::getX() {
    return posX_;
}

int Voiture::getY() {
    return posY_;
}

void Voiture::set_speed(const float newSpeed) {
    speed_ = newSpeed;
}

void Voiture::move() {

    posX_ += static_cast<float>(cos(angle_ * PI / 180.0) * speed_);
    posY_ += static_cast<float>(sin(angle_ * PI / 180.0) * speed_);
    spriteVoiture_.setPosition(this->getX(), this->getY());

}

void Voiture::turn() {

    switch (spawn_) {
    case Spawn_area::UP:
        if (this->getY() == 332 && turning_ == Turning::TURN_LEFT) {

        }
        if (this->getY() == 332 && turning_ == Turning::TURN_RIGHT) {

        }
        break;
    case Spawn_area::DOWN:
        if (this->getY() == 332) {

        }
        break;
    case Spawn_area::LEFT:
        break;
    case Spawn_area::RIGHT:
        break;
    }
    switch (turning_) {
    case Turning::NO_TURN:

    }

}

void Voiture::stop() {



}
