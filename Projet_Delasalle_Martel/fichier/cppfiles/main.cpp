
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

    /************************************** TEST **************************************/
    float radius = 10; // Rayon du cercle
    float l1 = 390; // Position en x du premier cercle
    float l2 = 470; // Position en y du premier cercle
    sf::CircleShape circle1(radius); // Crée un cercle s'appelant cercle1 de rayon 10 pixels
    circle1.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
    circle1.setOrigin(circle1.getRadius() / 2, circle1.getRadius() / 2); // Définit l'origine du cercle
    circle1.setPosition(l2 + radius / 2, l2 - 95 + radius / 2); // Définit la position du cercle dans la fenêtre
    sf::CircleShape circle2(radius);
    circle2.setFillColor(sf::Color::Green);
    circle2.setOrigin(circle2.getRadius() / 2, circle2.getRadius() / 2);
    circle2.setPosition(l2 + radius / 2, l1 - 95 - radius);
    sf::CircleShape circle3(radius); // Crée un cercle s'appelant cercle1 de rayon 10 pixels
    circle3.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
    circle3.setOrigin(circle3.getRadius() / 2, circle3.getRadius() / 2); // Définit l'origine du cercle
    circle3.setPosition(l2 - 90 + radius / 2, l2 - 95 + radius / 2); // Définit la position du cercle dans la fenêtre
    sf::CircleShape circle4(radius); 
    circle4.setFillColor(sf::Color::Green); 
    circle4.setOrigin(circle4.getRadius() / 2, circle4.getRadius() / 2);
    circle4.setPosition(l2 - 90 + radius / 2, l1 - 95 - radius);

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

        /************************************** TEST **************************************/
        window.draw(circle1); // Dessine le cercle1
        window.draw(circle2); // Dessine le cercle2
        window.draw(circle3); // Dessine le cercle3
        window.draw(circle4); // Dessine le cercle4 

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