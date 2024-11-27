#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include "TrafficLight.hpp"
#include "Voiture.hpp"

// Définition de la couleur orange
static const sf::Color Orange(255, 165, 0);

int main() {
    sf::RenderWindow window(sf::VideoMode(877, 669), "Simulation de Feux de Circulation");
    sf::Texture mapTexture, carTexture;
    if (!mapTexture.loadFromFile("../../../../img/map.png") || !carTexture.loadFromFile("../../../../img/voiture.png")) {
        std::cerr << "Erreur lors du chargement des images." << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite mapSprite(mapTexture);

    TrafficLight trafficLightMaster(TrafficColor::Red), trafficLightSlave(TrafficColor::Green);
    const auto timeWaiting = std::chrono::seconds(2);
    std::thread trafficLightThread([&]() {
        while (window.isOpen()) {
            std::this_thread::sleep_for(timeWaiting);
            trafficLightMaster.update();
            trafficLightSlave.update();
        }
        });

    float radius = 10;
    sf::CircleShape circle1(radius), circle2(radius), circle3(radius), circle4(radius);
    circle1.setPosition(470 + radius / 2, 470 - 95 + radius / 2);
    circle2.setPosition(470 + radius / 2, 390 - 95 - radius);
    circle3.setPosition(470 - 90 + radius / 2, 470 - 95 + radius / 2);
    circle4.setPosition(470 - 90 + radius / 2, 390 - 95 - radius);

    const float stopXRight = 503, stopXLeft = 374;
    std::vector<Voiture> carsForward, carsBackward;
    const float carSpeed = 0.1f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> delayDist(500, 1500), turnDist(0, 1);

    sf::Clock forwardClock, backwardClock;
    int forwardDelay = delayDist(gen), backwardDelay = delayDist(gen);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        if (backwardClock.getElapsedTime().asMilliseconds() >= backwardDelay) {
            carsBackward.emplace_back(carTexture, false);
            backwardDelay = delayDist(gen);
            backwardClock.restart();
        }

        if (forwardClock.getElapsedTime().asMilliseconds() >= forwardDelay) {
            carsForward.emplace_back(carTexture, true);
            forwardDelay = delayDist(gen);
            forwardClock.restart();
        }

        // Gérer les voitures en marche arrière
        for (auto it = carsBackward.begin(); it != carsBackward.end();) {
            float currentX = it->sprite.getPosition().x;
            float currentY = it->sprite.getPosition().y;
            bool canMove = true;

            if (it != carsBackward.begin()) {
                auto prevIt = std::prev(it);
                if (!prevIt->turnRight && currentX - prevIt->sprite.getPosition().x < 40) {
                    canMove = false; // La voiture ne peut pas avancer si trop proche de la précédente
                }
            }

            // Vérifie si le feu est vert avant de permettre aux voitures de se déplacer
            if (trafficLightSlave.getColor() != TrafficColor::Green &&
                currentX <= stopXRight + 10 && currentX > stopXRight - 10) {
                canMove = false; // Si le feu n'est pas vert et que la voiture est dans la zone d'arrêt, elle doit s'arrêter
            }

            // Prise de décision pour tourner ou continuer tout droit
            it->makeDecision(turnDist, gen, currentX, currentY);

            // La voiture peut se déplacer uniquement si elle est autorisée par le feu
            if (canMove) {
                it->move(-carSpeed, it->turnRight, it->turnLeft);
            }

            // Si la voiture quitte la fenêtre, on l'efface
            if (currentX <= 3 || currentY <= 5) {
                it = carsBackward.erase(it);
            }
            else {
                ++it;
            }
        }

        // Gérer les voitures en marche avant
        for (auto it = carsForward.begin(); it != carsForward.end();) {
            float currentX = it->sprite.getPosition().x;
            float currentY = it->sprite.getPosition().y;
            bool canMove = true;

            if (it != carsForward.begin()) {
                auto prevIt = std::prev(it);
                if (!prevIt->turnRight && prevIt->sprite.getPosition().x - currentX < 40) {
                    canMove = false; // La voiture ne peut pas avancer si trop proche de la précédente
                }
            }

            // Vérifie si le feu est vert avant de permettre aux voitures de se déplacer
            if (trafficLightSlave.getColor() != TrafficColor::Green &&
                currentX >= stopXLeft - 10 && currentX < stopXLeft + 10) {
                canMove = false; // Si le feu n'est pas vert et que la voiture est dans la zone d'arrêt, elle doit s'arrêter
            }

            // Prise de décision pour tourner ou continuer tout droit
            it->makeDecision(turnDist, gen, currentX, currentY);

            // La voiture peut se déplacer uniquement si elle est autorisée par le feu
            if (canMove) {
                it->move(carSpeed, it->turnRight, it->turnLeft);  
            }

            // Si la voiture quitte la fenêtre, on l'efface
            if (currentX >= 871 || currentY >= 659) {
                it = carsForward.erase(it);
            }
            else {
                ++it;
            }
        }

        circle1.setFillColor(trafficLightMaster.getSfmlColor());
        circle2.setFillColor(trafficLightSlave.getSfmlColor());
        circle3.setFillColor(trafficLightSlave.getSfmlColor());
        circle4.setFillColor(trafficLightMaster.getSfmlColor());

        window.clear(sf::Color::Black);
        window.draw(mapSprite);
        window.draw(circle1);
        window.draw(circle2);
        window.draw(circle3);
        window.draw(circle4);
        for (const auto& car : carsForward) window.draw(car.sprite);
        for (const auto& car : carsBackward) window.draw(car.sprite);
        window.display();
    }

    if (trafficLightThread.joinable()) trafficLightThread.join();
    return EXIT_SUCCESS;
}
