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
        sprite.move(0, speed); // Tourne à droite
    }
    else if (turnLeftDirection){ 
        sprite.move(0, -speed); // Tourne à gauche
    }
    else {
        sprite.move(speed, 0); // Continue tout droit
    }
}

void Voiture::makeDecision(std::uniform_int_distribution<int>& turnDist, std::mt19937& gen, float x, float y) {
    // Si la voiture atteint une certaine position et n'a pas encore pris de décision
    if ( !decisionTaken && x >= 435 && x < 439 && y == 339) {
        turnRight = (turnDist(gen) == 1);  // Décision aléatoire de tourner à droite 
        turnLeft = (turnDist(gen) == 1);  // Décision aléatoire de tourner à gauche
        if (turnRight && x >= 435 && x < 439 && y == 339) {
                decisionTaken = true;
                sprite.setRotation(180);  // Tourner à 360° (vers la droite)
                sprite.setPosition(435, 359);  // Nouvelle position après le virage à droite
        }
        else if (turnLeft) {
                decisionTaken = true;
                sprite.setRotation(0);  // Tourner à 0° (vers la gauche)
                sprite.setPosition(440, 334);  // Nouvelle position après le virage à gauche
        }
    }

    //if ( (!decisionTaken && x == 440 && y == 332) || (!decisionTaken && x == 435 && y ==332) ) {
    //    if (x == 440 && y == 332) {
    //        turnRight = (turnDist(gen) == 1);  // Décision aléatoire de tourner à droite 
    //        if (turnRight && x == 440 && y == 332) {
    //            decisionTaken = true;
    //            sprite.setRotation(360);  // Tourner à 360° (vers la droite)
    //            sprite.setPosition(440, 302);  // Nouvelle position après le virage à droite
    //        }
    //    }
    //    if (x == 435 && y == 332) {
    //        turnLeft = (turnDist(gen) == 1);  // Décision aléatoire de tourner à gauche
    //        if (turnLeft && x == 435 && y == 332) {
    //            decisionTaken = true;
    //            sprite.setRotation(0);  // Tourner à 0° (vers la gauche)
    //            sprite.setPosition(435, 340);  // Nouvelle position après le virage à gauche
    //        }
    //    }
    //}
}
