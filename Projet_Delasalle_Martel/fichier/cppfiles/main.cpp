#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>

int main() {
    // Crée une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(877, 669), "Animation de voitures");

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

    // Listes pour les voitures qui avancent et reculent
    std::vector<sf::Sprite> carsForward;
    std::vector<sf::Sprite> carsBackward;

    // Vitesse des voitures
    const float speed = 0.1f;

    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> delayDist(500, 1500); // Délais aléatoires entre 500 ms et 1500 ms

    // Horloges pour les délais d'apparition des voitures
    sf::Clock forwardClock;
    sf::Clock backwardClock;

    int forwardDelay = delayDist(gen);  // Temps d'attente initial pour les voitures avant
    int backwardDelay = delayDist(gen); // Temps d'attente initial pour les voitures arrière

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Ajouter une voiture allant de gauche à droite (si délai écoulé)
        if (forwardClock.getElapsedTime().asMilliseconds() >= forwardDelay) {
            sf::Sprite carForward(carTexture);
            carForward.setScale(0.1f, 0.1f);
            carForward.setPosition(3, 339);
            carForward.setRotation(90); // Tourne la voiture de 90 degrés à droite
            carsForward.push_back(carForward);
            forwardDelay = delayDist(gen); // Génère un nouveau délai aléatoire
            forwardClock.restart();
        }

        // Ajouter une voiture allant de droite à gauche (si délai écoulé)
        if (backwardClock.getElapsedTime().asMilliseconds() >= backwardDelay) {
            sf::Sprite carBackward(carTexture);
            carBackward.setScale(0.1f, 0.1f);
            carBackward.setPosition(871, 332);
            carBackward.setRotation(-90); // Tourne la voiture de 90 degrés à gauche
            carsBackward.push_back(carBackward);
            backwardDelay = delayDist(gen); // Génère un nouveau délai aléatoire
            backwardClock.restart();
        }

        // Mise à jour des positions des voitures et suppression si destination atteinte
        for (auto it = carsForward.begin(); it != carsForward.end();) {
            it->move(speed, 0); // Déplace vers la droite
            if (it->getPosition().x >= 871) {
                it = carsForward.erase(it); // Supprime la voiture arrivée à destination
            }
            else {
                ++it;
            }
        }
        for (auto it = carsBackward.begin(); it != carsBackward.end();) {
            it->move(-speed, 0); // Déplace vers la gauche
            if (it->getPosition().x <= 3) {
                it = carsBackward.erase(it); // Supprime la voiture arrivée à destination
            }
            else {
                ++it;
            }
        }

        // Efface la fenêtre
        window.clear(sf::Color::Black);

        // Dessine le sprite de fond
        window.draw(mapSprite);

        // Dessine les voitures
        for (const auto& car : carsForward) {
            window.draw(car);
        }
        for (const auto& car : carsBackward) {
            window.draw(car);
        }

        // Affiche les changements
        window.display();
    }

    return EXIT_SUCCESS;
}
