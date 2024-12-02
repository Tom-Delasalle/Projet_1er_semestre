#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <cmath>
#include "OnFoot.hpp"

# define PI 3.14159265358979323846

using namespace std;
OnFoot::OnFoot(const float speed, const Spawn_area& spawn, const Turning& turning) : spawn_(spawn), turning_(turning), speed_(speed) {

    // Charge l'image du piéton
    if (!imageOnFootM.loadFromFile("../../../../img/OnFootM.png")) {
        cerr << "Erreur : Impossible de charger l'image du piéton.png\n";
    }
    spriteOnFootM.setTexture(imageOnFootM);

    switch (spawn) {
    case Spawn_area::UP:
        posX_ = 415;
        posY_ = 3;
        angle_ = 180.f;
        break;
    case Spawn_area::DOWN:
        posX_ = 425;
        posY_ = 699;
        angle_ = 270.f;
        break;
    case Spawn_area::LEFT:
        posX_ = 3;
        posY_ = 379;
        angle_ = 0.f;
        break;
    case Spawn_area::RIGHT:
        posX_ = 871;
        posY_ = 292;
        angle_ = 90.f;
        break;
    default:
        posX_ = 254;
        posY_ = 254;
        angle_ = 0.f;
        cerr << "Erreur : La position x y des voitures n'a pas pu se faire correctement\n";
    }
    spriteOnFootM.setRotation(angle_);
}

int OnFoot::getX() {
    return posX_;
}

int OnFoot::getY() {
    return posY_;
}

void OnFoot::set_speed(const float newSpeed) {
    speed_ = newSpeed;
}

void OnFoot::move() {

    posX_ += static_cast<float>(cos(angle_ * PI / 180.0) * speed_);
    posY_ += static_cast<float>(sin(angle_ * PI / 180.0) * speed_);
    spriteOnFootM.setPosition(this->getX(), this->getY());

}

void OnFoot::turn() {

    switch (spawn_) {
    case Spawn_area::UP: // original angle : 90
        if (this->getX() <= 500 && this->getY() >= 275 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 1.f;
            if (angle_ < 0.f) {
                angle_ = 0.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        if (this->getX() >= 300 && this->getY() >= 275 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 1.f;
            if (angle_ > 180.f) {
                angle_ = 180.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        break;
    case Spawn_area::DOWN: // original angle : 270
        if (this->getX() >= 300 && this->getY() <= 390 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 1.f;
            if (angle_ < 180.f) {
                angle_ = 180.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        if (this->getX() <= 500 && this->getY() <= 390 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 1.f;
            if (angle_ > 360.f) {
                angle_ = 360.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        break;
    case Spawn_area::LEFT: // original angle : 0
        if (this->getX() >= 382 && this->getY() >= 300 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 1.f;
            if (angle_ < -90.f) {
                angle_ = -90.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        if (this->getX() <= 382 && this->getY() <= 500 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 1.f;
            if (angle_ > 90.f) {
                angle_ = 90.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        break;
    case Spawn_area::RIGHT: // original angle : 180
        if (this->getX() <= 494 && this->getY() <= 500 && turning_ == Turning::TURN_LEFT) {
            angle_ -= 1.f;
            if (angle_ < 90.f) {
                angle_ = 90.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        if (this->getX() <= 494 && this->getY() >= 300 && turning_ == Turning::TURN_RIGHT) {
            angle_ += 1.f;
            if (angle_ > 270.f) {
                angle_ = 270.f;
            }
            spriteOnFootM.setRotation(angle_);
        }
        break;
    }
}

void OnFoot::stop() {



}