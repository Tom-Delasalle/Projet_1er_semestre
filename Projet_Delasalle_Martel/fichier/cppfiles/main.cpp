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

private:
    TrafficColor currentColor;
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
    std::vector<sf::Sprite> carsForward, carsBackward;
    const float carSpeed = 0.1f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> delayDist(500, 1500);

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
            sf::Sprite carForward(carTexture);
            carForward.setScale(0.1f, 0.1f);
            carForward.setPosition(3, 339);
            carForward.setRotation(90);
            carsForward.push_back(carForward);
            forwardDelay = delayDist(gen);
            forwardClock.restart();
        }

        if (backwardClock.getElapsedTime().asMilliseconds() >= backwardDelay) {
            sf::Sprite carBackward(carTexture);
            carBackward.setScale(0.1f, 0.1f);
            carBackward.setPosition(871, 332);
            carBackward.setRotation(-90);
            carsBackward.push_back(carBackward);
            backwardDelay = delayDist(gen);
            backwardClock.restart();
        }

        // Mise à jour des positions des voitures venant de la gauche (stop au feu 354, 339)
        for (auto it = carsForward.begin(); it != carsForward.end();) {
            float currentX = it->getPosition().x;
            bool canMove = true;

            // Vérifie la distance avec la voiture devant
            if (it != carsForward.begin()) {
                auto prevIt = std::prev(it);
                if (prevIt->getPosition().x - currentX < 40) {
                    canMove = false;
                }
            }

            // Vérifie le feu et la zone d'arrêt
            if (trafficLightSlave.getSfmlColor() == sf::Color::Red && currentX >= stopXLeft - 10 && currentX < stopXLeft + 10) {
                canMove = false;
            }

            // Déplace la voiture si possible
            if (canMove) {
                it->move(carSpeed, 0);
            }

            // Supprime les voitures hors de la fenêtre
            if (currentX >= 871) {
                it = carsForward.erase(it);
            }
            else {
                ++it;
            }
        }

        // Mise à jour des positions des voitures venant de la droite (stop au feu 523, 332)
        for (auto it = carsBackward.begin(); it != carsBackward.end();) {
            float currentX = it->getPosition().x;
            bool canMove = true;

            // Vérifie la distance avec la voiture devant
            if (it != carsBackward.begin()) {
                auto prevIt = std::prev(it);
                if (currentX - prevIt->getPosition().x < 40) {
                    canMove = false;
                }
            }

            // Vérifie le feu et la zone d'arrêt
            if (trafficLightMaster.getSfmlColor() == sf::Color::Red && currentX <= stopXRight + 10 && currentX > stopXRight - 10) {
                canMove = false;
            }

            // Déplace la voiture si possible
            if (canMove) {
                it->move(-carSpeed, 0);
            }

            // Supprime les voitures hors de la fenêtre
            if (currentX <= 3) {
                it = carsBackward.erase(it);
            }
            else {
                ++it;
            }
        }

        // Mise à jour des feux
        circle1.setFillColor(trafficLightSlave.getSfmlColor());
        circle2.setFillColor(trafficLightMaster.getSfmlColor());
        circle3.setFillColor(trafficLightSlave.getSfmlColor());
        circle4.setFillColor(trafficLightMaster.getSfmlColor());

        // Affichage
        window.clear(sf::Color::Black);
        window.draw(mapSprite);
        //window.draw(circle1);
        window.draw(circle2);
        window.draw(circle3);
        //window.draw(circle4);
        for (const auto& car : carsForward) {
            window.draw(car);
        }
        for (const auto& car : carsBackward) {
            window.draw(car);
        }
        window.display();
    }

    if (trafficLightThread.joinable()) {
        trafficLightThread.join();
    }

    return EXIT_SUCCESS;
}