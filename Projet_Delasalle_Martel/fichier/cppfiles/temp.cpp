#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

// D�finition de la couleur orange
static const sf::Color Orange(255, 165, 0);

// Enum pour repr�senter les couleurs des feux
enum class TrafficColor { Green, Orange, Red };

// Classe pour repr�senter un feu de circulation
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
        return sf::Color::Black; // D�faut, ne devrait jamais arriver
    }

    TrafficColor getColor() const {
        return currentColor;
    }

private:
    TrafficColor currentColor;
};

// Classe pour repr�senter une voiture
class Voiture {
public:
    sf::Sprite sprite;
    bool decisionTaken = false; // Si la voiture a d�j� pris une d�cision (tourner ou continuer)
    bool turnRight = false;     // D�cision : true si la voiture doit tourner � droite

    Voiture(const sf::Texture& texture, float x, float y, float rotation) {
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(x, y);
        sprite.setRotation(rotation);
    }

    void move(float speed, bool turnRightDirection) {
        if (turnRightDirection) {
            sprite.move(0, speed); // Descend si tourne � droite
        }
        else {
            sprite.move(speed, 0); // Continue tout droit
        }
    }

    void makeDecision(std::uniform_int_distribution<int>& turnDist, std::mt19937& gen, float x, float y) {
        if (!decisionTaken && x >= 425 && x < 430 && y == 339) {
            decisionTaken = true;
            turnRight = (turnDist(gen) == 1); // D�cision al�atoire de tourner � droite
            if (turnRight) {
                sprite.setRotation(180); // Tourne � 180� pour descendre
                sprite.setPosition(425, 359); // Position du virage
            }
        }
    }
};

int main() {
    // Cr�e une fen�tre SFML
    sf::RenderWindow window(sf::VideoMode(877, 669), "Simulation de Feux de Circulation");
    window.setFramerateLimit(60);

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

    // Feux tricolores (ma�tre et esclave)
    TrafficLight trafficLightMaster(TrafficColor::Red);
    TrafficLight trafficLightSlave(TrafficColor::Green);

    // Param�tres des feux de circulation
    const auto timeWaiting = std::chrono::seconds(2); // Temps d'attente par �tat
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

    // Zones d'arr�t pour les feux
    const float stopXRight = 503; // Zone d'arr�t pour les voitures venant de la droite
    const float stopXLeft = 374;  // Zone d'arr�t pour les voitures venant de la gauche

    // Listes des voitures
    std::vector<Voiture> carsForward, carsVector;
    const float carSpeed = 1.2f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> delayDist(500, 1500);
    std::uniform_int_distribution<int> turnDist(0, 1); // Al�atoire pour le virage

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
            Voiture carForward(carTexture, 3, 339, 90);
            carsForward.push_back(carForward);
            forwardDelay = delayDist(gen);
            forwardClock.restart();
        }

        if (backwardClock.getElapsedTime().asMilliseconds() >= backwardDelay) {
            Voiture carBackward(carTexture, 871, 332, -90);
            carsVector.push_back(carBackward);
            backwardDelay = delayDist(gen);
            backwardClock.restart();
        }

        // Mise � jour des positions des voitures venant de la droite
        for (auto it = carsVector.begin(); it != carsVector.end();) {
            float currentX = it->sprite.getPosition().x;
            float currentY = it->sprite.getPosition().y;
            bool canMove = true;

            if (it != carsVector.begin()) {
                auto prevIt = std::prev(it);
                if (!prevIt->turnRight && currentX - prevIt->sprite.getPosition().x < 40) {
                    canMove = false; // La voiture ne peut pas avancer si trop proche de la pr�c�dente
                }
            }

            // V�rifie si le feu est vert avant de permettre aux voitures de se d�placer
            if (trafficLightSlave.getColor() != TrafficColor::Green &&
                currentX <= stopXRight + 10 && currentX > stopXRight - 10) {
                canMove = false; // Si le feu n'est pas vert et que la voiture est dans la zone d'arr�t, elle doit s'arr�ter
            }

            // Prise de d�cision pour tourner ou continuer tout droit
            it->makeDecision(turnDist, gen, currentX, currentY);

            // La voiture peut se d�placer uniquement si elle est autoris�e par le feu
            if (canMove) {
                it->move(carSpeed, it->turnRight);
            }

            // Si la voiture quitte la fen�tre, on l'efface
            if (currentX <= 3 || currentY <= 5) {
                it = carsVector.erase(it);
            }
            else {
                ++it;
            }
        }
        // Mise � jour des positions des voitures venant de la gauche
        for (auto it = carsForward.begin(); it != carsForward.end();) {
            float currentX = it->sprite.getPosition().x;
            float currentY = it->sprite.getPosition().y;
            bool canMove = true;

            if (it != carsForward.begin()) {
                auto prevIt = std::prev(it);
                if (!prevIt->turnRight && prevIt->sprite.getPosition().x - currentX < 40) {
                    canMove = false; // La voiture ne peut pas avancer si trop proche de la pr�c�dente
                }
            }

            // V�rifie si le feu est vert avant de permettre aux voitures de se d�placer
            if (trafficLightSlave.getColor() != TrafficColor::Green &&
                currentX >= stopXLeft - 10 && currentX < stopXLeft + 10) {
                canMove = false; // Si le feu n'est pas vert et que la voiture est dans la zone d'arr�t, elle doit s'arr�ter
            }

            // Prise de d�cision pour tourner ou continuer tout droit
            it->makeDecision(turnDist, gen, currentX, currentY);

            // La voiture peut se d�placer uniquement si elle est autoris�e par le feu
            if (canMove) {
                it->move(carSpeed, it->turnRight);
            }

            // Si la voiture quitte la fen�tre, on l'efface
            if (currentX >= 871 || currentY >= 659) {
                it = carsForward.erase(it);
            }
            else {
                ++it;
            }
        }

        // Mise � jour des feux
        circle1.setFillColor(trafficLightMaster.getSfmlColor()); // Feu ma�tre pour la droite
        circle2.setFillColor(trafficLightSlave.getSfmlColor());  // Feu esclave pour la gauche
        circle3.setFillColor(trafficLightSlave.getSfmlColor());  // Feu esclave pour la droite
        circle4.setFillColor(trafficLightMaster.getSfmlColor()); // Feu ma�tre pour la gauche

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
        for (const auto& car : carsVector) {
            window.draw(car.sprite);
        }
        window.display();
    }

    if (trafficLightThread.joinable()) {
        trafficLightThread.join();
    }

    return EXIT_SUCCESS;
}


for (i; i < 6; ++i) {
    cout << "New car spawned ";
    switch (spawnAndTurnRand(gen)) {
    case 1: spawn = Spawn_area::UP; cout << "at the TOP    "; break;
    case 2: spawn = Spawn_area::DOWN;  cout << "at the BOTTOM "; break;
    case 3: spawn = Spawn_area::LEFT; cout << "to the LEFT   "; break;
    default: spawn = Spawn_area::RIGHT; cout << "to the RIGHT  ";
    }
    switch (spawnAndTurnRand(gen)) {
    case 1: turn = Turning::TURN_LEFT; cout << "turning LEFT\n"; break;
    case 2: turn = Turning::TURN_RIGHT; cout << "turning RIGHT\n"; break;
    default: turn = Turning::NO_TURN; cout << "NOT turning\n";
    }
    switch (i) {
    case 0: spawn1 = spawn; turn1 = turn; break;
    case 1: spawn2 = spawn; turn2 = turn; break;
    case 2: spawn3 = spawn; turn3 = turn; break;
    case 3: spawn4 = spawn; turn4 = turn; break;
    case 4: spawn5 = spawn; turn5 = turn; break;
    case 5: spawn6 = spawn; turn6 = turn;
    }
}