#include "Voiture.hpp"

Voiture::Voiture(const sf::Texture& texture, bool forward) {
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
    if (forward) {
        sprite.setPosition(spawnXForward, spawnYForward);
        sprite.setRotation(90);  // Rotation pour avancer vers la droite
    }
    else {
        sprite.setPosition(spawnXBackward, spawnYBackward);
        sprite.setRotation(-90); // Rotation pour avancer vers la gauche
    }
}

void Voiture::move(float speed, bool turnRightDirection) {
    if (turnRightDirection) {
        sprite.move(0, speed); // Descend si tourne à droite
    }
    else {
        sprite.move(speed, 0); // Continue tout droit
    }
}

void Voiture::makeDecision(std::uniform_int_distribution<int>& turnDist, std::mt19937& gen, float x, float y) {
    if (!decisionTaken && x >= 425 && x < 430 && y == 339) {
        decisionTaken = true;
        turnRight = (turnDist(gen) == 1);
        if (turnRight) {
            sprite.setRotation(180);
            sprite.setPosition(425, 359);
        }
    }
    if (!decisionTaken && x >= 455 && x < 460 && y == 332) {
        decisionTaken = true;
        turnRight = (turnDist(gen) == 1);
        if (turnRight) {
            sprite.setRotation(180);
            sprite.setPosition(455, 332);
        }
    }
}
