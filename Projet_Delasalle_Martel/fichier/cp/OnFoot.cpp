#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "OnFoot.hpp"
#include <chrono>

#define PI 3.14159265358979323846

using namespace std;
using namespace chrono_literals; // Permet de faire des opération de temps avec s, min, h, .

OnFoot::OnFoot(const float speed, const sf::Texture& imagePieton, const Spawn_area spawn, const Turning turning)
    : spawn_(spawn), turning_(turning), speed_(speed), imagePieton_(ref(imagePieton)) {

    moving_ = Moving::ON_FOOT;

    spritePieton_.setTexture(imagePieton);
    if (spritePieton_.getTexture() == nullptr) {
        cerr << "Erreur : Pieton sans texture\n";
    }
    spritePieton_.setOrigin(92.f, 160.f);
    spritePieton_.setScale(0.1f, 0.1f);

    posX_ = switch_posX(moving_, spawn_);
    posY_ = switch_posY(moving_, spawn_);
    angle_ = switch_angle(moving_, spawn_);
    spritePieton_.setPosition(posX_, posY_);
    spritePieton_.setRotation(angle_);

    radiusCollision_ = 15.f;
    pedestrianAndCenterGap_ = 25.f;
    centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;
    centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;

}



void OnFoot::Respawn(const Spawn_area& spawn, const Turning& turning) {

    turning_ = turning;
    spawn_ = spawn;

    posX_ = switch_posX(moving_, spawn_);
    posY_ = switch_posY(moving_, spawn_);
    angle_ = switch_angle(moving_, spawn_);

    spritePieton_.setPosition(posX_, posY_);
    spritePieton_.setRotation(angle_);

    centerCollisionX_ = posX_ + static_cast<float>(cos((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;
    centerCollisionY_ = posY_ + static_cast<float>(sin((angle_ - 90) * PI / 180.0)) * speed_ * pedestrianAndCenterGap_;

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

}

void OnFoot::turn() {

    //if (turning_ != Turning::NO_TURN) {

    //    switch (spawn_) {
    //    case Spawn_area::UP: // original angle : 180
    //        if (this->getX() <= 500 && this->getY() >= 365 && turning_ == Turning::TURN_LEFT) {
    //            angle_ -= 2.f;
    //            if (angle_ < 90.f) {
    //                angle_ = 90.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        if (this->getX() >= 300 && this->getY() >= 250 && turning_ == Turning::TURN_RIGHT) {
    //            angle_ += 2.f;
    //            if (angle_ > 270.f) {
    //                angle_ = 270.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        break;
    //    case Spawn_area::DOWN: // original angle : 0
    //        if (this->getX() >= 300 && this->getY() <= 300 && turning_ == Turning::TURN_LEFT) {
    //            angle_ -= 2.f;
    //            if (angle_ < -90.f) {
    //                angle_ = -90.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        if (this->getX() <= 500 && this->getY() <= 420 && turning_ == Turning::TURN_RIGHT) {
    //            angle_ += 2.f;
    //            if (angle_ > 90.f) {
    //                angle_ = 90.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        break;
    //    case Spawn_area::LEFT: // original angle : 90
    //        if (this->getX() >= 370 && this->getY() >= 300 && turning_ == Turning::TURN_LEFT) {
    //            angle_ -= 2.f;
    //            if (angle_ < 0.f) {
    //                angle_ = 0.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        if (this->getX() >= 450 && this->getY() <= 500 && turning_ == Turning::TURN_RIGHT) {
    //            angle_ += 2.f;
    //            if (angle_ > 180.f) {
    //                angle_ = 180.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        break;
    //    case Spawn_area::RIGHT: // original angle : -90
    //        if (this->getX() <= 463 && this->getY() <= 500 && turning_ == Turning::TURN_LEFT) {
    //            angle_ -= 2.f;
    //            if (angle_ < -180.f) {
    //                angle_ = -180.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        if (this->getX() <= 470 && this->getY() >= 300 && turning_ == Turning::TURN_RIGHT) {
    //            angle_ += 2.f;
    //            if (angle_ > 0.f) {
    //                angle_ = 0.f;
    //            }
    //            spritePieton_.setRotation(angle_);
    //        }
    //        break;
    //    }
    //}
}

// Vérifie que l'origine d'un piéton n'est pas à l'intérieur du cercle de collision
bool OnFoot::isNotClose(const float otherPosX, const float otherPosY) {

    if (pow((otherPosX - centerCollisionX_), 2.f) + pow((otherPosY - centerCollisionY_), 2.f) <= pow(radiusCollision_, 2.f)) {
        cout << "Pedestrian is in collision zone\n";
        return false;
    }
    else {
        return true;
    }

}

//void OnFoot::stop() {
//
//
//
//}
