#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "voiture.hpp"

#define PI 3.14159265358979323846

using namespace std;
using namespace chrono_literals; // Permet de faire des opération de temps avec s, min, h, ...

Voiture::Voiture(const float speed, const sf::Texture& imageVoiture, const Spawn_area& spawn, const Turning& turning) 
    : spawn_(spawn), turning_(turning), speed_(speed), imageVoiture_(ref(imageVoiture)) {

    spriteVoiture_.setTexture(imageVoiture);
    spriteVoiture_.setScale(0.1f, 0.1f);
    
    switch (spawn) { // Change les valeurs de la position x et y et de l'angle du sprite en fonction de l'endroit où va apparaître la voiture
    case Spawn_area::UP:
        posX_ = 455.f;
        posY_ = 3.f;
        angle_ = 90.f;
        break;
    case Spawn_area::DOWN:
        posX_ = 425.f;
        posY_ = 659.f;
        angle_ = 270.f;
        break;
    case Spawn_area::LEFT:
        posX_ = 3.f;
        posY_ = 339.f;
        angle_ = 0.f;
        break;
    case Spawn_area::RIGHT:
        posX_ = 871.f;
        posY_ = 332.f;
        angle_ = 180.f;
        break;
    default:
        posX_ = 254.f;
        posY_ = 254.f;
        angle_ = 0.f;
        cerr << "Erreur : La position x y des voitures n'a pas pu se faire correctement\n";
    }
    spriteVoiture_.setPosition(posX_, posY_);
    spriteVoiture_.setRotation(angle_);

}

float Voiture::getX() {
    return posX_;
}

float Voiture::getY() {
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
    case Spawn_area::UP: // original angle : 90
        if (this->getX() <= 500 && this->getY() >= 332 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 0.0001f;
            if (angle_ < 0.f) {
                angle_ = 0.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        if (this->getX() >= 300 && this->getY() >= 332 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 0.0001f;
            if (angle_ > 180.f) {
                angle_ = 180.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        break;
    case Spawn_area::DOWN: // original angle : 270
        if (this->getX() >= 300 && this->getY() <= 339 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 0.0001f;
            if (angle_ < 180.f) {
                angle_ = 180.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        if (this->getX() <= 500 && this->getY() <= 339 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 0.0001f;
            if (angle_ > 360.f) {
                angle_ = 360.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        break;
    case Spawn_area::LEFT: // original angle : 0
        if (this->getX() >= 354 && this->getY() >= 300 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 0.0001f;
            if (angle_ < -90.f) {
                angle_ = -90.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        if (this->getX() <= 354 && this->getY() <= 500 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 0.0001f;
            if (angle_ > 90.f) {
                angle_ = 90.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        break;
    case Spawn_area::RIGHT: // original angle : 180
        if (this->getX() <= 523 && this->getY() <= 500 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 0.0001f;
            if (angle_ < 90.f) {
                angle_ = 90.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        if (this->getX() <= 523 && this->getY() >= 300 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 0.0001f;
            if (angle_ > 270.f) {
                angle_ = 270.f;
            }
            spriteVoiture_.setRotation(angle_);
        }
        break;
    }

}

//void Voiture::stop() {
//
//
//
//}
