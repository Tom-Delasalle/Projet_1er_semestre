#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

int main() {

    // Crée une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(877, 669), "Fenêtre SFML");

    // Charge la texture
    sf::Texture texture;
    if (!texture.loadFromFile("../../../../img/map.png")) {
        std::cerr << "Erreur : Impossible de charger l'image map.png" << std::endl;
        return EXIT_FAILURE;
    }

    // Associe la texture au sprite
    sf::Sprite sprite(texture);



    /************************************** TEST **************************************/
    float radius = 10; // Rayon du cercle
    float l1 = 390; // Position en x du premier cercle
    float l2 = 470; // Position en y du premier cercle
    sf::CircleShape circle1(radius); // Crée un cercle s'appelant cercle1 de rayon 10 pixels
    circle1.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
    circle1.setOrigin(circle1.getRadius() / 2, circle1.getRadius() / 2); // Définit l'origine du cercle
    circle1.setPosition(l2 + radius / 2, l2-95 + radius / 2); // Définit la position du cercle dans la fenêtre
    sf::CircleShape circle2(radius);
    circle2.setFillColor(sf::Color::Green);
    circle2.setOrigin(circle2.getRadius() / 2, circle2.getRadius() / 2);
    circle2.setPosition(l2 + radius / 2, l1-95 - radius);
    sf::CircleShape circle3(radius); // Crée un cercle s'appelant cercle1 de rayon 10 pixels
    circle3.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
    circle3.setOrigin(circle3.getRadius() / 2, circle3.getRadius() / 2); // Définit l'origine du cercle
    circle3.setPosition(l2-90 + radius / 2, l2 - 95 + radius / 2); // Définit la position du cercle dans la fenêtre
    sf::CircleShape circle4(radius);
    circle4.setFillColor(sf::Color::Green);
    circle4.setOrigin(circle4.getRadius() / 2, circle4.getRadius() / 2);
    circle4.setPosition(l2-90 + radius / 2, l1 - 95 - radius);




    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Efface la fenêtre
        window.clear(sf::Color::Black);

        // Dessine le sprite
        window.draw(sprite);



        /************************************** TEST **************************************/
        window.draw(circle1); // Dessine le cercle1
        window.draw(circle2); // Dessine le cercle2
        window.draw(circle3); // Dessine le cercle3
        window.draw(circle4); // Dessine le cercle4

        // Affiche les changements
        window.display();
    }

    return EXIT_SUCCESS;
}

