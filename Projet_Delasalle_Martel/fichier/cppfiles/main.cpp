#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

int main() {
    // Cr�e une fen�tre SFML
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

    // D�lais d'apparition des voitures
    sf::Clock clock;
    const sf::Time delayBetweenCars = sf::seconds(1.0f); // D�lai de 1 seconde
    int carsCreated = 0; // Compteur des voitures cr��es

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Ajouter une nouvelle voiture si le d�lai est �coul� et que le total est < 10
        if (carsCreated < 10 && clock.getElapsedTime() > delayBetweenCars) {
            // Ajoute une voiture allant de gauche � droite
            sf::Sprite carForward(carTexture);
            carForward.setScale(0.1f, 0.1f);
            carForward.setPosition(3, 346);
            carForward.setRotation(90); // Tourne la voiture de 90 degr�s � droite
            carsForward.push_back(carForward);

            // Ajoute une voiture allant de droite � gauche
            sf::Sprite carBackward(carTexture);
            carBackward.setScale(0.1f, 0.1f);
            carBackward.setPosition(871, 322);
            carBackward.setRotation(-90); // Tourne la voiture de 90 degr�s � gauche
            carsBackward.push_back(carBackward);

            carsCreated++; // Incr�mente le compteur des voitures cr��es
            clock.restart(); // R�initialise le d�lai
        }

        // Mise � jour des positions des voitures et suppression si destination atteinte
        for (auto it = carsForward.begin(); it != carsForward.end();) {
            it->move(speed, 0); // D�place vers la droite
            if (it->getPosition().x >= 871) {
                it = carsForward.erase(it); // Supprime la voiture arriv�e � destination
            }
            else {
                ++it;
            }
        }
        for (auto it = carsBackward.begin(); it != carsBackward.end();) {
            it->move(-speed, 0); // D�place vers la gauche
            if (it->getPosition().x <= 3) {
                it = carsBackward.erase(it); // Supprime la voiture arriv�e � destination
            }
            else {
                ++it;
            }
        }

        // Efface la fen�tre
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
