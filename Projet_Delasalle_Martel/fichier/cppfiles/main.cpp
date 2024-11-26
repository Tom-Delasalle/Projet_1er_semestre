#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

// Définition de la couleur orange
static const sf::Color Orange(255, 165, 0);

// Enum pour représenter les couleurs des feux
enum class TrafficColor { Green, Orange, Red };

// Classe pour représenter un feu de circulation
class TrafficLight {
public:
    TrafficLight(TrafficColor color) : currentColor(color) {}

    void update() {
        switch (currentColor) {
        case TrafficColor::Green:
            currentColor = TrafficColor::Orange;
            break;
        case TrafficColor::Orange:
            currentColor = TrafficColor::Red;
            break;
        case TrafficColor::Red:
            currentColor = TrafficColor::Green;
            break;
        }
    }

    const sf::Color& getSfmlColor() const {
        switch (currentColor) {
        case TrafficColor::Green:
            return sf::Color::Green;
        case TrafficColor::Orange:
            return Orange;
        case TrafficColor::Red:
            return sf::Color::Red;
        }
        return sf::Color::Black; // Défaut, ne devrait jamais arriver
    }

    TrafficColor getColor() const {
        return currentColor;
    }

private:
    TrafficColor currentColor;
};

// Structure pour représenter une voiture
struct Car {
    sf::Sprite sprite;
    bool decisionTaken = false; // Si la voiture a déjà pris une décision (tourner ou continuer)
    bool turnRight = false;     // Décision : true si la voiture doit tourner à droite
};

int main() {
    // Crée une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(877, 669), "Simulation de Feux de Circulation");

    // Charge la texture du fond
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../../../../img/map.png")) {
        std::cerr << "Erreur : Impossible de charger l'image map.png" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite mapSprite(mapTexture);

    // Charge la texture des voitures
    sf::Texture carTexture;
    if (!carTexture.loadFromFile("../../../../img/voiture.png")) {
        std::cerr << "Erreur : Impossible de charger l'image voiture.png" << std::endl;
        return EXIT_FAILURE;
    }

    // Feux tricolores (maître et esclave)
    TrafficLight trafficLightMaster(TrafficColor::Red);
    TrafficLight trafficLightSlave(TrafficColor::Green);

    // Paramètres des feux de circulation
    const auto timeWaiting = std::chrono::seconds(2); // Temps d'attente par état
    std::thread trafficLightThread([&]() {
        while (window.isOpen()) {
            std::this_thread::sleep_for(timeWaiting);
            trafficLightMaster.update();
            trafficLightSlave.update();
        }
        });

    // Positions des feux
    float radius = 10; // Rayon des cercles
    sf::CircleShape circle1(radius), circle2(radius), circle3(radius), circle4(radius);

    circle1.setPosition(470 + radius / 2, 470 - 95 + radius / 2);
    circle2.setPosition(470 + radius / 2, 390 - 95 - radius);
    circle3.setPosition(470 - 90 + radius / 2, 470 - 95 + radius / 2);
    circle4.setPosition(470 - 90 + radius / 2, 390 - 95 - radius);

    // Zones d'arrêt pour les feux
    const float stopXRight = 503; // Zone d'arrêt pour les voitures venant de la droite
    const float stopXLeft = 374;  // Zone d'arrêt pour les voitures venant de la gauche

    // Listes des voitures
    std::vector<Car> carsForward, carsBackward;
    const float carSpeed = 0.1f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> delayDist(500, 1500);
    std::uniform_int_distribution<int> turnDist(0, 1); // Aléatoire pour le virage

    // Horloges pour l'apparition des voitures
    sf::Clock forwardClock, backwardClock;
    int forwardDelay = delayDist(gen), backwardDelay = delayDist(gen);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Apparition des voitures
        if (forwardClock.getElapsedTime().asMilliseconds() >= forwardDelay) {
            Car carForward;
            carForward.sprite.setTexture(carTexture);
            carForward.sprite.setScale(0.1f, 0.1f);
            carForward.sprite.setPosition(3, 339);
            carForward.sprite.setRotation(90);
            carsForward.push_back(carForward);
            forwardDelay = delayDist(gen);
            forwardClock.restart();
        }

        if (backwardClock.getElapsedTime().asMilliseconds() >= backwardDelay) {
            Car carBackward;
            carBackward.sprite.setTexture(carTexture);
            carBackward.sprite.setScale(0.1f, 0.1f);
            carBackward.sprite.setPosition(871, 332);
            carBackward.sprite.setRotation(-90);
            carsBackward.push_back(carBackward);
            backwardDelay = delayDist(gen);
            backwardClock.restart();
        }

        // Mise à jour des positions des voitures venant de la gauche
        for (auto it = carsForward.begin(); it != carsForward.end();) {
            float currentX = it->sprite.getPosition().x;
            float currentY = it->sprite.getPosition().y;
            bool canMove = true;

            if (it != carsForward.begin()) {
                auto prevIt = std::prev(it);
                if (!prevIt->turnRight && prevIt->sprite.getPosition().x - currentX < 40) {
                    canMove = false;
                }
            }

            if ((trafficLightSlave.getColor() == TrafficColor::Orange || trafficLightSlave.getColor() == TrafficColor::Red) &&
                currentX >= stopXLeft - 10 && currentX < stopXLeft + 10) {
                canMove = false;
            }

            // Prise de décision au point critique
            if (!it->decisionTaken && currentX >= 425 && currentX < 430 && currentY == 339) {
                it->decisionTaken = true;
                it->turnRight = (turnDist(gen) == 1);
                if (it->turnRight) {
                    it->sprite.setRotation(180); // Tourne vers le bas
                    it->sprite.setPosition(425, 359);
                }
            }

            if (canMove) {
                if (it->turnRight) {
                    it->sprite.move(0, carSpeed); // Descend
                }
                else {
                    it->sprite.move(carSpeed, 0); // Continue tout droit
                }
            }

            if (currentX >= 871 || currentY >= 659) {
                it = carsForward.erase(it);
            }
            else {
                ++it;
            }
        }

        // Mise à jour des positions des voitures venant de la droite
        for (auto it = carsBackward.begin(); it != carsBackward.end();) {
            float currentX = it->sprite.getPosition().x;
            float currentY = it->sprite.getPosition().y;
            bool canMove = true;

            if (it != carsBackward.begin()) {
                auto prevIt = std::prev(it);
                if (!prevIt->turnRight && currentX - prevIt->sprite.getPosition().x < 40) {
                    canMove = false;
                }
            }

            if ((trafficLightMaster.getColor() == TrafficColor::Orange || trafficLightMaster.getColor() == TrafficColor::Red) &&
                currentX <= stopXRight + 10 && currentX > stopXRight - 10) {
                canMove = false;
            }

            if (!it->decisionTaken && currentX <= 455 && currentX > 450 && currentY == 332) {
                it->decisionTaken = true;
                it->turnRight = (turnDist(gen) == 1);
                if (it->turnRight) {
                    it->sprite.setRotation(180);
                    it->sprite.setPosition(455, 332);
                }
            }

            if (canMove) {
                if (it->turnRight) {
                    it->sprite.move(0, -carSpeed);
                }
                else {
                    it->sprite.move(-carSpeed, 0);
                }
            }

            if (currentX <= 3 || currentY <= 5) {
                it = carsBackward.erase(it);
            }
            else {
                ++it;
            }
        }

        // Mise à jour des feux
        circle1.setFillColor(trafficLightMaster.getSfmlColor()); // Feu maître pour la droite
        circle2.setFillColor(trafficLightSlave.getSfmlColor());  // Feu esclave pour la gauche
        circle3.setFillColor(trafficLightSlave.getSfmlColor());  // Feu esclave pour la droite
        circle4.setFillColor(trafficLightMaster.getSfmlColor()); // Feu maître pour la gauche


        // Affichage
        window.clear(sf::Color::Black);
        window.draw(mapSprite);
        window.draw(circle2);
        window.draw(circle3);
        window.draw(circle1); 
        window.draw(circle4);
        for (const auto& car : carsForward) {
            window.draw(car.sprite);
        }
        for (const auto& car : carsBackward) {
            window.draw(car.sprite);
        }
        window.display();
    }

    if (trafficLightThread.joinable()) {
        trafficLightThread.join();
    }

    return EXIT_SUCCESS;
}
