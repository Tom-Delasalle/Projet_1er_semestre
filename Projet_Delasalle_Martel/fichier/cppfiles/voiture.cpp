#include <iostream>
#include <thread>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "carrefour.hpp"
#include "tricolore.hpp"

using namespace std;
using namespace chrono_literals; // Permet de faire des opération de temps avec s, min, h, ...

static const sf::Color Orange(255, 165, 0); // Crée la couleur orange à partir des valeurs RGB

const sf::Color& get_SFML_color(const Traffic_light& traffic_light) // Fonction qui retourne la couleur actuelle du feu tricolore dans la class Color
{
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::green:
        return sf::Color::Green;
    case Traffic_color::red:
        return sf::Color::Red;
    }
    return Orange;
}

ostream& operator<<(ostream& os, const Traffic_light& traffic_light) // Surcharge de l'opérateur << (pour print) quand la class Traffic_light
{                                                                              // est donnée afin de retourner un simple string
    string ptr;
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::red:
        ptr = "Red";
        break;
    case Traffic_color::green:
        ptr = "Green";
        break;
    case Traffic_color::orange:
        ptr = "Orange";
    }
    os << ptr;
    return os;
}

void run_traffic_light(Traffic_light& traffic_light_master, Traffic_light& traffic_light_slave, stop_token stop_token)
{
    traffic_light_master.set_traffic_color(Traffic_color::green); // Donne la couleur de départ verte au feu tricolore maître
    traffic_light_slave.set_traffic_color(Traffic_color::red);    // Donne la couleur de départ rouge au feu tricolore esclave
    while (!stop_token.stop_requested())
    {
        this_thread::sleep_for(time_waiting);
        if (traffic_light_master.get_traffic_color() == Traffic_color::green) // Si la couleur du feu tricolore mâitre est verte
        {
            ++traffic_light_master; // Fait passer le feu tricolore maître au orange puis au rouge
            ++traffic_light_slave;  // Fait passer le feu tricolore esclave au vert une fois que le feu tricolore maître est passé au rouge
        }
        else
        {
            ++traffic_light_slave;  // Fait passer le feu tricolore esclave au orange puis au rouge
            ++traffic_light_master; // Fait passer le feu tricolore maître au vert une foisque le feu tricolore esclave est passé au rouge
        }
    }
}

void print_traffic_light(Traffic_light& traffic_light_master, Traffic_light& traffic_light_slave, stop_token stop_token)
{
    while (!stop_token.stop_requested())
    {
        this_thread::sleep_for(1s);
        cout << "Taffic light master : " << traffic_light_master << " Taffic light slave : " << traffic_light_slave << endl;
    }
}

int main() {

    stop_source stopping; // Crée stopping de la classe stop_source. Cela permet de générer de requêtes d'arrêts 
    Traffic_light traffic_light_master{ Traffic_color::red }; // Crée le feu tricolore maître est esclave et les initialise
    Traffic_light traffic_light_slave{ Traffic_color::red };  // avec la couleur rouge par défaut
    jthread thread_traffic_light_master1(run_traffic_light,
        ref(traffic_light_master), ref(traffic_light_slave), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(800, 800), "My window"); // Crée une fenêtre "My window" de dessin 2D SFML de 800 x 800 pixels 

    float l1 = 390, l2 = 470, radius = 10;
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

    while (window.isOpen()) // Tant que la fenêtre est ouverte
    {
        sf::Event event; // Crée un event qui permet de stocker des évènements en attente
        while (window.pollEvent(event)) // Tant qu'il y a des évènements en attente dans la file d'attente de évènements de la fenêtre
        {
            if (event.type == sf::Event::Closed) //  Si l'événement est la fermeture de la fenêtre
            {
                stopping.request_stop(); // Envoie une requête d'arrêt à tous les stop_token créés
                window.close(); // Ferme la fenêtre
                return 0;
            }
        }
        window.clear(sf::Color::Black); // Clear la fenêtre et affiche du noir

        circle1.setFillColor(get_SFML_color(traffic_light_slave)); // Change la couleur de fond du cercle1
        circle2.setFillColor(get_SFML_color(traffic_light_master));
        circle3.setFillColor(get_SFML_color(traffic_light_slave));
        circle4.setFillColor(get_SFML_color(traffic_light_master));
        window.draw(circle1); // Dessine le cercle1
        window.draw(circle2);
        window.draw(circle3);
        window.draw(circle4);

        window.display(); // Affiche la fenêtre
    }

    return EXIT_SUCCESS;

}