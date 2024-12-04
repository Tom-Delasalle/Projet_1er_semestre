#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>
#include <mutex>
#include "OnFoot.hpp"

# define PI 3.14159265358979323846

using namespace std;

OnFoot::OnFoot(const float speed, const Spawn_area& spawn, const Turning& turning)
    : spawn_(spawn), turning_(turning), speed_(speed), isMoving_(true) {

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
    // Avance le piéton selon son angle et sa vitesse
    posX_ += static_cast<float>(cos(angle_ * PI / 180.0) * speed_);
    posY_ += static_cast<float>(sin(angle_ * PI / 180.0) * speed_);
    spriteOnFootM.setPosition(this->getX(), this->getY());
}

void OnFoot::stop() {
    isMoving_ = false;  // Arrêter le mouvement du piéton
}

void OnFoot::moveAutomatically() {
    while (isMoving_) {
        move();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Avancer toutes les 100 ms
    }
}

void OnFoot::startMoving() {
    // Démarrer le thread pour le mouvement automatique
    movementThread = std::thread(&OnFoot::moveAutomatically, this);
}

void OnFoot::joinThread() {
    if (movementThread.joinable()) {
        movementThread.join();
    }
}
