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

void Voiture::move(float speed, bool turnRightDirection, bool turnLeftDirection) {
    if (turnRightDirection) {
        sprite.move(0, speed); // Tourne � droite
    }
    else if (turnLeftDirection){ 
        sprite.move(0, -speed); // Tourne � gauche
    }
    else {
        sprite.move(speed, 0); // Continue tout droit
    }
}

void Voiture::makeDecision(std::uniform_int_distribution<int>& turnDist, std::mt19937& gen, float x, float y) {
    // Si la voiture atteint une certaine position et n'a pas encore pris de d�cision
    if (!decisionTaken && x >= 435 && x < 440 && y == 339) {
        decisionTaken = true;
        turnRight = (turnDist(gen) == 1);  // D�cision al�atoire de tourner � droite ou � gauche
        turnLeft = (turnDist(gen) == 1);  // D�cision al�atoire de tourner � gauche
        if (turnRight) {
            sprite.setRotation(180);  // Tourner � 180� (vers la gauche)
            sprite.setPosition(435, 359);  // Nouvelle position apr�s le virage � gauche
        }
        else if (turnLeft) {
            sprite.setRotation(0);  // Tourner � 90� (vers la droite)
            sprite.setPosition(405, 359);  // Nouvelle position apr�s le virage � droite
        }
    }

    if (!decisionTaken && x >= 455 && x < 460 && y == 332) {
        decisionTaken = true;
        turnRight = (turnDist(gen) == 1);  // D�cision al�atoire de tourner � droite 
        turnLeft = (turnDist(gen) == 1);  // D�cision al�atoire de tourner � gauche
        if (turnRight) {
            sprite.setRotation(360);  // Tourner � 360� (vers la droite)
            sprite.setPosition(440, 302);  // Nouvelle position apr�s le virage � droite
        }
        else if (turnLeft) {
            sprite.setRotation(180);  // Tourner � 0� (vers la gauche)
            sprite.setPosition(420, 302);  // Nouvelle position apr�s le virage � gauche
        }
    }
}
