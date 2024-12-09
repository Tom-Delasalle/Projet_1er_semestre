#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tricolore.hpp"
#include "voiture.hpp"

using namespace std;
using namespace chrono_literals;

// Constantes
const float carSpeed = 0.0001f;
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> carDelay(1500, 2500);
uniform_int_distribution<int> spawnAndTurnRand(1, 4);

// Variables globales
mutex carMutex; // Protéger l'accès à carsVector

static const sf::Color Orange(255, 165, 0);

const sf::Color& get_SFML_color(const Traffic_light& traffic_light) {
    switch (traffic_light.get_traffic_color()) {
    case Traffic_color::green:
        return sf::Color::Green;
    case Traffic_color::red:
        return sf::Color::Red;
    }
    return Orange;
}

void run_traffic_light(Traffic_light& traffic_light_master, Traffic_light& traffic_light_slave, stop_token stop_token) {
    traffic_light_master.set_traffic_color(Traffic_color::green);
    traffic_light_slave.set_traffic_color(Traffic_color::red);
    while (!stop_token.stop_requested()) {
        this_thread::sleep_for(2s); // Temps de cycle
        if (traffic_light_master.get_traffic_color() == Traffic_color::green) {
            ++traffic_light_master;
            ++traffic_light_slave;
        }
        else {
            ++traffic_light_slave;
            ++traffic_light_master;
        }
    }
}

void moving_cars(vector<Voiture>& carsVector, sf::Texture& carTexture, sf::Clock& carClock, int& spawnDelay, stop_token stopToken) {
    while (!stopToken.stop_requested()) {
        if (carClock.getElapsedTime().asMilliseconds() >= spawnDelay || carsVector.empty()) {
            Spawn_area spawn = static_cast<Spawn_area>(spawnAndTurnRand(gen) % 4 + 1);
            Turning turn = static_cast<Turning>(spawnAndTurnRand(gen) % 3);
            Voiture newCar(carSpeed, carTexture, spawn, turn);

            // Verrouille l'accès à carsVector
            {
                lock_guard<mutex> lock(carMutex);
                carsVector.push_back(newCar);
            }

            spawnDelay = carDelay(gen);
            carClock.restart();
        }

        // Mise à jour des voitures
        lock_guard<mutex> lock(carMutex); // Protège carsVector
        for (auto it = carsVector.begin(); it != carsVector.end();) {
            it->turn();
            it->move();

            // Supprime les voitures qui sortent de la fenêtre
            if (it->getX() <= 0 || it->getX() >= 877 || it->getY() <= 0 || it->getY() >= 669) {
                it = carsVector.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

int main() {
    stop_source stopping;

    vector<Voiture> carsVector;

    sf::Clock carClock;
    int spawnDelay = carDelay(gen);

    sf::Texture carTexture;
    if (!carTexture.loadFromFile("../../../../img/voiture.png")) {
        cerr << "Erreur : Impossible de charger l'image voiture.png\n";
        return EXIT_FAILURE;
    }

    jthread carThread(moving_cars, ref(carsVector), ref(carTexture), ref(carClock), ref(spawnDelay), stopping.get_token());

    Traffic_light traffic_light_master{ Traffic_color::red };
    Traffic_light traffic_light_slave{ Traffic_color::red };
    jthread trafficLightThread(run_traffic_light, ref(traffic_light_master), ref(traffic_light_slave), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(877, 669), "Carrefour Vauban");

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../../../../img/map.png")) {
        cerr << "Erreur : Impossible de charger l'image map.png\n";
        return EXIT_FAILURE;
    }
    sf::Sprite mapSprite(mapTexture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                stopping.request_stop();
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(mapSprite);

        // Dessine toutes les voitures
        {
            lock_guard<mutex> lock(carMutex); // Protège l'accès à carsVector
            for (const auto& car : carsVector) {
                if (car.spriteVoiture_.getTexture() == nullptr) {
                    cerr << "Erreur : Voiture sans texture\n";
                    continue;
                }
                window.draw(car.spriteVoiture_);
            }
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
