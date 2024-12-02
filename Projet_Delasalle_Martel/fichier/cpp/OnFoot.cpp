#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "OnFoot.hpp"

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
        break;
    case Spawn_area::DOWN:
        posX_ = 425;
        posY_ = 699;
        break;
    case Spawn_area::LEFT:
        posX_ = 3;
        posY_ = 379;
        break;
    case Spawn_area::RIGHT:
        posX_ = 871;
        posY_ = 292;
        break;
    default:
        posX_ = 254;
        posY_ = 254;
        cerr << "Erreur : La position x y des voitures n'a pas pu se faire correctement\n";
    }

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
        cerr << "Erreur : il y a un probleme avec la fonction move() du piéton\n";
    }

}

void OnFoot::stop() {



}

void OnFoot::turn() {



}