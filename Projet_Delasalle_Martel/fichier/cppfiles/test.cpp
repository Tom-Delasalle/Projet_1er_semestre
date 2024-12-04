#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "carrefour.hpp"
#include "tricolore.hpp"
#include "voiture.hpp"

const float stopXLeft = 374.f;    // Zone d'arrêt pour les voitures venant de la gauche
const float stopXRight = 503.f;   // Zone d'arrêt pour les voitures venant de la droite
const float stopYUp = 300.f;      // Zone d'arrêt pour les voitures venant du haut
const float stopYBottom = 500.f;  // Zone d'arrêt pour les voitures venant du haut
const float carSpeed = 0.00005f;    // Vitesse des voitures

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> carDelay(1500, 2500);
uniform_int_distribution<int> spawnAndTurnRand(1, 4);

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

// Thread function for moving the cars
void moving_cars(vector<Voiture>& carsVector,
    sf::Texture& imageVoiture,
    Turning& turn,
    Spawn_area& spawn,
    sf::Clock& carClock,
    int spawnDelay,
    stop_token stopToken) {
    
    while (!stopToken.stop_requested()) {
        // Check si le temps écoulé est égal ou supérieur à la limite donné de façon aléatoire ou si le vecteur est vide et qu'il n'y a pas de demande d'arrêt
        if (carClock.getElapsedTime().asMilliseconds() >= spawnDelay || carsVector.empty()) {
            cout << "New car spawned ";
            switch (spawnAndTurnRand(gen)) {
            case 1:
                spawn = Spawn_area::UP;
                cout << "at the top ";
                break;
            case 2:
                spawn = Spawn_area::DOWN;
                cout << "at the bottom ";
                break;
            case 3:
                spawn = Spawn_area::LEFT;
                cout << "to the left ";
                break;
            default:
                spawn = Spawn_area::RIGHT;
                cout << "to the right ";
            }
            switch (spawnAndTurnRand(gen)) {
            case 1:
                turn = Turning::TURN_LEFT;
                cout << "turning left\n";
                break;
            case 2:
                turn = Turning::TURN_RIGHT;
                cout << "turning right\n";
                break;
            default:
                turn = Turning::NO_TURN;
                cout << "not turning\n";
            }
            Voiture carSingle(carSpeed, ref(imageVoiture), spawn, turn); // Créé une nouvelle voiture
            carsVector.push_back(carSingle); // Push dans le vecteur
            spawnDelay = carDelay(gen); // Nouveau délai pour spawn la prochaine voiture
            carClock.restart(); // On remet l'horloge à zéro
        }

        for (auto it = carsVector.begin(); it != carsVector.end();) {
            float currentX = it->getX();
            float currentY = it->getY();
            bool canMove = true;

            /* Vérifie si le feu est vert avant de permettre aux voitures de se déplacer
            if (trafficLightSlave.getColor() != TrafficColor::Green &&
                currentX <= stopXRight + 10 && currentX > stopXRight - 10) {
                canMove = false; // Si le feu n'est pas vert et que la voiture est dans la zone d'arrêt, elle doit s'arrêter
            }*/

            // La voiture peut se déplacer uniquement si elle est autorisée par le feu
            if (canMove) {
                it->turn();
                it->move();
            }

            // Si la voiture quitte la fenêtre, on l'efface
            if (currentX <= 2 || currentX >= 873 || currentY <= 2 || currentY >= 661) {
                it = carsVector.erase(it);
                cout << "Deleted car\n";
            }
            else {
                ++it;
            }
        }
    }
    

}

int main() {

    stop_source stopping; // Crée stopping de la classe stop_source. Cela permet de générer de requêtes d'arrêts 

    // Listes des voitures
    vector<Voiture> carsVector;

    Turning turn;
    Spawn_area spawn;

    // Horloges pour l'apparition des voitures
    sf::Clock carClock;
    int spawnDelay = carDelay(gen);

    // Charge l'image de la voiture
    sf::Texture imageVoiture;
    if (!imageVoiture.loadFromFile("../../../../img/voiture.png")) {
        cerr << "Erreur : Impossible de charger l'image voiture.png\n";
        return EXIT_FAILURE;
    }

    jthread jthread_moving_cars(moving_cars,
        ref(carsVector),
        ref(imageVoiture),
        ref(turn),
        ref(spawn),
        ref(carClock),
        spawnDelay,
        stopping.get_token());

    Traffic_light traffic_light_master{ Traffic_color::red }; // Crée le feu tricolore maître et esclave et les initialise
    Traffic_light traffic_light_slave{ Traffic_color::red };  // avec la couleur rouge par défaut
    jthread thread_traffic_light_master(run_traffic_light,
        ref(traffic_light_master), ref(traffic_light_slave), stopping.get_token());

    //sf::RenderWindow window(sf::VideoMode(800, 800), "My window"); Crée une fenêtre "My window" de dessin 2D SFML de 800 x 800 pixels 
    sf::RenderWindow window(sf::VideoMode(877, 669), "Carrefour Vauban");

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../../../../img/map.png")) {
        cerr << "Error : Unable to load map.png" << endl;
        return EXIT_FAILURE;
    }
    sf::Sprite mapSprite(mapTexture);

    float l1 = 390, l2 = 470, size = 800, radius = 10;
    //sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(0, l1)), sf::Vertex(sf::Vector2f(size, l1)) };
    // Crée une ligne s'appelant line1 qui commence au point (0, 350) et se termine au point (800, 350) de couleur blanc (par défaut)
    sf::CircleShape circle1(radius); // Crée un cercle s'appelant cercle1 de rayon 10 pixels
    circle1.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
    circle1.setOrigin(circle1.getRadius() / 2, circle1.getRadius() / 2); // Définit l'origine du cercle
    circle1.setPosition(l2 + 30 + radius / 2, l2 - 60 + radius / 2); // Définit la position du cercle dans la fenêtre
    sf::CircleShape circle2(radius);
    circle2.setFillColor(sf::Color::Green);
    circle2.setOrigin(circle2.getRadius() / 2, circle2.getRadius() / 2);
    circle2.setPosition(l2 + 40 + radius / 2, l1 - 125 - radius);
    sf::CircleShape circle3(radius);
    circle3.setFillColor(sf::Color::Blue);
    circle3.setOrigin(circle3.getRadius() / 2, circle3.getRadius() / 2);
    circle3.setPosition(l2 - 120 + radius / 2, l1 - 135 - radius);
    sf::CircleShape circle4(radius);
    circle4.setFillColor(sf::Color::Green);
    circle4.setOrigin(circle4.getRadius() / 2, circle4.getRadius() / 2);
    circle4.setPosition(l2 - 130 + radius / 2, l2 - 65 + radius / 2);

    while (window.isOpen()) // Tant que la fenêtre est ouverte
    {
        sf::Event event; // Crée un event qui permet de stocker des évènements en attente
        while (window.pollEvent(event)) // Tant qu'il y a des évènements en attente dans la file d'attente de évènements de la fenêtre
        {
            if (event.type == sf::Event::Closed) // Si l'événement est la fermeture de la fenêtre
            {
                stopping.request_stop(); // Envoie une requête d'arrêt à tous les stop_token créés
                window.close(); // Ferme la fenêtre
                return 0;
            }
        }
        window.clear(sf::Color::Black); // Clear la fenêtre et affiche du noir
        window.draw(mapSprite);         // Dessine le sprite de fond

        //window.draw(line1, 2, sf::Lines); // Dessine la line1
        circle1.setFillColor(get_SFML_color(traffic_light_slave)); // Change la couleur de fond du cercle1
        circle2.setFillColor(get_SFML_color(traffic_light_master));
        circle3.setFillColor(get_SFML_color(traffic_light_slave));
        circle4.setFillColor(get_SFML_color(traffic_light_master));
        window.draw(circle1); // Dessine le cercle1
        window.draw(circle2);
        window.draw(circle3);
        window.draw(circle4);

        // Affiche toutes les voitures
        for (const auto& car : carsVector) {
            window.draw(car.spriteVoiture_);
        }

        window.display(); // Affiche la fenêtre
    }

    return EXIT_SUCCESS;

}