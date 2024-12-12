#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tricolore.hpp"
#include "voiture.hpp"

const float stopXLeft = 374.f;    // Zone d'arr�t pour les voitures venant de la gauche
const float stopXRight = 503.f;   // Zone d'arr�t pour les voitures venant de la droite
const float stopYUp = 300.f;      // Zone d'arr�t pour les voitures venant du haut
const float stopYBottom = 500.f;  // Zone d'arr�t pour les voitures venant du haut
const float carSpeed = 30.0f;    // Vitesse des voitures

// G�n�rateur random
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> carDelay(1500, 2500);
uniform_int_distribution<int> spawnAndTurnRand(1, 4);
// Prot�ger l'acc�s � carsVector
mutex carMutex;

using namespace std;
using namespace chrono_literals; // Permet de faire des op�ration de temps avec s, min, h, ...

static const sf::Color Orange(255, 165, 0); // Cr�e la couleur orange � partir des valeurs RGB

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

ostream& operator<<(ostream& os, const Traffic_light& traffic_light) // Surcharge de l'op�rateur << (pour print) quand la class Traffic_light
{                                                                              // est donn�e afin de retourner un simple string
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
    traffic_light_master.set_traffic_color(Traffic_color::green); // Donne la couleur de d�part verte au feu tricolore ma�tre
    traffic_light_slave.set_traffic_color(Traffic_color::red);    // Donne la couleur de d�part rouge au feu tricolore esclave
    while (!stop_token.stop_requested())
    {
        this_thread::sleep_for(time_waiting);
        if (traffic_light_master.get_traffic_color() == Traffic_color::green) // Si la couleur du feu tricolore m�itre est verte
        {
            ++traffic_light_master; // Fait passer le feu tricolore ma�tre au orange puis au rouge
            ++traffic_light_slave;  // Fait passer le feu tricolore esclave au vert une fois que le feu tricolore ma�tre est pass� au rouge
        }
        else
        {
            ++traffic_light_slave;  // Fait passer le feu tricolore esclave au orange puis au rouge
            ++traffic_light_master; // Fait passer le feu tricolore ma�tre au vert une foisque le feu tricolore esclave est pass� au rouge
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
void moving_cars(Voiture& car,
    Spawn_area& spawn,
    Turning& turn,
    stop_token stopToken) {

    sf::Clock carClock;
    float currentX = car.getX();
    float currentY = car.getY();

    while (!stopToken.stop_requested()) {

        float deltaTime = carClock.restart().asSeconds();
        currentX = car.getX();
        currentY = car.getY();
        bool canMove = true;

        /* V�rifie si le feu est vert avant de permettre aux voitures de se d�placer
        if (trafficLightSlave.getColor() != TrafficColor::Green &&
            currentX <= stopXRight + 10 && currentX > stopXRight - 10) {
            canMove = false; // Si le feu n'est pas vert et que la voiture est dans la zone d'arr�t, elle doit s'arr�ter
        }*/

        // La voiture peut se d�placer uniquement si elle est autoris�e par le feu
        if (canMove) {
            car.move(deltaTime);
            car.turn();
        }


        // Si la voiture quitte la fen�tre, on l'efface
        if (currentX <= 0 || currentX >= 875 || currentY <= 0 || currentY >= 663) {
            cout << "Respawned a car ";
            switch (spawnAndTurnRand(gen)) {
            case 1: spawn = Spawn_area::UP; cout << "at the top "; break;
            case 2: spawn = Spawn_area::DOWN;  cout << "at the bottom "; break;
            case 3: spawn = Spawn_area::LEFT; cout << "to the left "; break;
            default: spawn = Spawn_area::RIGHT; cout << "to the right ";
            }
            switch (spawnAndTurnRand(gen)) {
            case 1: turn = Turning::TURN_LEFT; cout << "turning left\n"; break;
            case 2: turn = Turning::TURN_RIGHT; cout << "turning right\n"; break;
            default:  turn = Turning::NO_TURN; cout << "not turning\n";
            }
            car.Respawn(spawn, turn);
        }

        //this_thread::sleep_for(chrono::microseconds(1));
    }

}

int main() {

    stop_source stopping; // Cr�e stopping de la classe stop_source. Cela permet de g�n�rer de requ�tes d'arr�ts 

    sf::Clock frameClock;

    // Listes des voitures
    vector<Voiture> carsVector;

    Turning turn, turn1, turn2, turn3, turn4, turn5, turn6;
    Spawn_area spawn, spawn1, spawn2, spawn3, spawn4, spawn5, spawn6;

    // Horloges pour l'apparition des voitures
    sf::Clock carClock;
    int spawnDelay = carDelay(gen);

    // Charge l'image de la voiture
    sf::Texture imageVoiture;
    if (!imageVoiture.loadFromFile("../../../../img/voiture.png")) {
        cerr << "Erreur : Impossible de charger l'image voiture.png\n";
        return EXIT_FAILURE;
    }

    int i = 0;
    for (i; i < 6; ++i) {
        cout << "New car spawned ";
        switch (spawnAndTurnRand(gen)) {
        case 1: spawn = Spawn_area::UP; cout << "at the top "; break;
        case 2: spawn = Spawn_area::DOWN;  cout << "at the bottom "; break;
        case 3: spawn = Spawn_area::LEFT; cout << "to the left "; break;
        default: spawn = Spawn_area::RIGHT; cout << "to the right ";
        }
        switch (spawnAndTurnRand(gen)) {
        case 1: turn = Turning::TURN_LEFT; cout << "turning left\n"; break;
        case 2: turn = Turning::TURN_RIGHT; cout << "turning right\n"; break;
        default: turn = Turning::NO_TURN; cout << "not turning\n";
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

    Voiture carSingle1(carSpeed, ref(imageVoiture), spawn1, turn1); // Cr�� une nouvelle voiture
    carsVector.push_back(carSingle1); // Push dans le vecteur
    Voiture carSingle2(carSpeed, ref(imageVoiture), spawn2, turn2); // Cr�� une nouvelle voiture
    carsVector.push_back(carSingle2); // Push dans le vecteur
    Voiture carSingle3(carSpeed, ref(imageVoiture), spawn3, turn3); // Cr�� une nouvelle voiture
    carsVector.push_back(carSingle3); // Push dans le vecteur
    Voiture carSingle4(carSpeed, ref(imageVoiture), spawn4, turn4); // Cr�� une nouvelle voiture
    carsVector.push_back(carSingle4); // Push dans le vecteur
    Voiture carSingle5(carSpeed, ref(imageVoiture), spawn5, turn5); // Cr�� une nouvelle voiture
    carsVector.push_back(carSingle5); // Push dans le vecteur
    Voiture carSingle6(carSpeed, ref(imageVoiture), spawn6, turn6); // Cr�� une nouvelle voiture
    carsVector.push_back(carSingle6); // Push dans le vecteur

    jthread jthread_moving_car1(moving_cars, ref(carsVector.at(0)), ref(spawn1), ref(turn1), stopping.get_token());
    jthread jthread_moving_car2(moving_cars, ref(carsVector.at(1)), ref(spawn2), ref(turn2), stopping.get_token());
    jthread jthread_moving_car3(moving_cars, ref(carsVector.at(2)), ref(spawn3), ref(turn3), stopping.get_token());
    jthread jthread_moving_car4(moving_cars, ref(carsVector.at(3)), ref(spawn4), ref(turn4), stopping.get_token());
    jthread jthread_moving_car5(moving_cars, ref(carsVector.at(4)), ref(spawn5), ref(turn5), stopping.get_token());
    jthread jthread_moving_car6(moving_cars, ref(carsVector.at(5)), ref(spawn6), ref(turn6), stopping.get_token());

    Traffic_light traffic_light_master{ Traffic_color::red }; // Cr�e le feu tricolore ma�tre et esclave et les initialise
    Traffic_light traffic_light_slave{ Traffic_color::red };  // avec la couleur rouge par d�faut
    jthread thread_traffic_light_master(run_traffic_light,
        ref(traffic_light_master), ref(traffic_light_slave), stopping.get_token());

    //sf::RenderWindow window(sf::VideoMode(800, 800), "My window"); Cr�e une fen�tre "My window" de dessin 2D SFML de 800 x 800 pixels 
    sf::RenderWindow window(sf::VideoMode(877, 669), "Carrefour Vauban");

    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../../../../img/map.png")) {
        cerr << "Error : Unable to load map.png" << endl;
        return EXIT_FAILURE;
    }
    sf::Sprite mapSprite(mapTexture);

    float l1 = 390, l2 = 470, size = 800, radius = 10;
    //sf::Vertex line1[] = { sf::Vertex(sf::Vector2f(0, l1)), sf::Vertex(sf::Vector2f(size, l1)) };
    // Cr�e une ligne s'appelant line1 qui commence au point (0, 350) et se termine au point (800, 350) de couleur blanc (par d�faut)
    sf::CircleShape circle1(radius); // Cr�e un cercle s'appelant cercle1 de rayon 10 pixels
    circle1.setFillColor(sf::Color::Blue); // L'aire du cercle est bleu
    circle1.setOrigin(circle1.getRadius() / 2, circle1.getRadius() / 2); // D�finit l'origine du cercle
    circle1.setPosition(l2 + 30 + radius / 2, l2 - 60 + radius / 2); // D�finit la position du cercle dans la fen�tre
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

    while (window.isOpen()) // Tant que la fen�tre est ouverte
    {
        sf::Event event; // Cr�e un event qui permet de stocker des �v�nements en attente
        while (window.pollEvent(event)) // Tant qu'il y a des �v�nements en attente dans la file d'attente de �v�nements de la fen�tre
        {
            if (event.type == sf::Event::Closed) // Si l'�v�nement est la fermeture de la fen�tre
            {
                stopping.request_stop(); // Envoie une requ�te d'arr�t � tous les stop_token cr��s
                window.close(); // Ferme la fen�tre
                return 0;
            }
        }
        window.clear(sf::Color::Black); // Clear la fen�tre et affiche du noir
        window.draw(mapSprite);         // Dessine le sprite de fond

        float deltaTime = frameClock.restart().asSeconds();

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
        lock_guard<mutex> lock(carMutex); // Prot�ge l'acc�s � carsVector
        for (const auto& car : carsVector) {
            if (car.spriteVoiture_.getTexture() == nullptr) {
                cerr << "Erreur : Voiture sans texture\n";
                continue;
            }
            window.draw(car.spriteVoiture_);
        }

        window.display(); // Affiche la fen�tre
    }

    return EXIT_SUCCESS;

}