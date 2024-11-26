#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "voiture.hpp"

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
        break;
    case Spawn_area::DOWN:
        posX_ = 425;
        posY_ = 659;
        break;
    case Spawn_area::LEFT:
        posX_ = 3;
        posY_ = 339;
        break;
    case Spawn_area::RIGHT:
        posX_ = 871;
        posY_ = 332;
        break;
    default:
        posX_ = 254;
        posY_ = 254;
        cerr << "Erreur : La position x y des voitures n'a pas pu se faire correctement\n";
    }

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

    switch (spawn_) {
    case Spawn_area::UP:
        break;
    case Spawn_area::DOWN:
        break;
    case Spawn_area::LEFT:
        break;
    case Spawn_area::RIGHT:
        break;
    default:
        cerr << "Erreur : il y a un probleme avec la fonction move() de la voiture\n";
    }

}

void Voiture::stop() {



}

void Voiture::turn() {



}