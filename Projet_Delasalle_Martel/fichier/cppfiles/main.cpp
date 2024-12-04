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

// Classe pour représenter une voiture
class Voiture {
public:
    sf::Sprite sprite;
    bool decisionTaken = false;
    bool turnRight = false;

    Voiture(const sf::Texture& texture, bool forward) {
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

    void move(float speed, bool turnRightDirection) {
        if (turnRightDirection) {
            sprite.move(0, speed); // Descend si tourne à droite
        }
        else {
            sprite.move(speed, 0); // Continue tout droit
        }
    }

    void makeDecision(std::uniform_int_distribution<int>& turnDist, std::mt19937& gen, float x, float y) {
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

private:
    static constexpr float spawnXForward = 3;
    static constexpr float spawnYForward = 339;
    static constexpr float spawnXBackward = 871;
    static constexpr float spawnYBackward = 332;
};

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
    std::vector<Voiture> carsForward, carsVector;
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
            carsVector.emplace_back(carTexture, false);
            backwardDelay = delayDist(gen);
            backwardClock.restart();
        }

        if (forwardClock.getElapsedTime().asMilliseconds() >= forwardDelay) {
            carsForward.emplace_back(carTexture, true);
            forwardDelay = delayDist(gen);
            forwardClock.restart();
        }

        for (auto it = carsVector.begin(); it != carsVector.end();) {
            float currentX = it->sprite.getPosition().x;
            float currentY = it->sprite.getPosition().y;
            bool canMove = true;

            if (it != carsVector.begin()) {
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
                it->move(-carSpeed, it->turnRight);
            }

            // Si la voiture quitte la fenêtre, on l'efface
            if (currentX <= 3 || currentY <= 5) {
                it = carsVector.erase(it);
            }
            else {
                ++it;
            }
        }

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
                it->move(carSpeed, it->turnRight);
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
        for (const auto& car : carsVector) window.draw(car.sprite);
        window.display();
    }

    if (trafficLightThread.joinable()) trafficLightThread.join();
    return EXIT_SUCCESS;
}
