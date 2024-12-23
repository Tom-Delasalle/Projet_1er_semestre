#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tricolore.hpp"
#include "voiture.hpp"
#include "bus.hpp"
#include "cycliste.hpp"
#include "OnFoot.hpp"

const float stopXLeft = 344.f;  // Zone d'arrêt pour les voitures venant de la gauche
const float stopXRight = 530.f; // Zone d'arrêt pour les voitures venant de la droite
const float stopYUp = 242.f;    // Zone d'arrêt pour les voitures venant du haut
const float stopYDown = 430.f;  // Zone d'arrêt pour les voitures venant du haut
const float carSpeed = 1.f;     // Vitesse des voitures
const float busSpeed = 1.f;     // Vitesse des bus
const float bikeSpeed = 0.8f;    // Vitesse des cyclistes
const float pedestrianSpeed = 1.f; //Vitesse des piétons


// Générateur random
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> spawnAndTurnRand(1, 4);
// Protéger l'accès à Vector
mutex carLock;
mutex busLock;
mutex bikeLock;
mutex pedestrianLock;

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
		cout << "Traffic light master : " << traffic_light_master << " Traffic light slave : " << traffic_light_slave << endl;
	}
}

// Thread function for moving the cars
void moving_cars(vector<Voiture>& carsVector,
	vector<Bus>& bussVector,
	vector<Cycliste>& bikesVector,
	vector<OnFoot>& pedestriansVector,
	Voiture& car,
	Traffic_light& traffic_light_master,
	Traffic_light& traffic_light_slave,
	Spawn_area spawn,
	Turning turn,
	chrono::seconds delayMove,
	stop_token stopToken) {

	bool canMove = true;

	this_thread::sleep_for(delayMove);

	while (!stopToken.stop_requested()) {

		// Switch pour check si la voiture se trouve dans la zone de détection du feu et si le feu est rouge ou orange. Si les conditions sont remplies, alors la voiture ne pourra pas plus avancer
		switch (spawn) {
		case Spawn_area::UP:
			if (car.getY() <= stopYUp && car.getY() >= stopYUp - 6.f && (traffic_light_slave.get_traffic_color() == Traffic_color::red || traffic_light_slave.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::DOWN:
			if (car.getY() >= stopYDown && car.getY() <= stopYDown + 6.f && (traffic_light_slave.get_traffic_color() == Traffic_color::red || traffic_light_slave.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::LEFT:
			if (car.getX() <= stopXLeft && car.getX() >= stopXLeft - 6.f && (traffic_light_master.get_traffic_color() == Traffic_color::red || traffic_light_master.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::RIGHT:
			if (car.getX() >= stopXRight && car.getX() <= stopXRight + 6.f && (traffic_light_master.get_traffic_color() == Traffic_color::red || traffic_light_master.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		}

		// Boucle pour appliquer la fonction isNotClose à chaque voiture présente
		for (int i = 0; i < carsVector.size(); ++i) {
			if ((carsVector.at(i).getX() != car.getX() || carsVector.at(i).getY() != car.getY()) && canMove) {
				canMove = car.isNotClose(Moving::CAR, carsVector.at(i).getX(), carsVector.at(i).getY());
			}
		}
		// Boucle pour appliquer la fonction isNotClose à chaque bus présent
		for (int j = 0; j < bussVector.size(); ++j) {
			if (canMove) {
				canMove = car.isNotClose(Moving::BUS, bussVector.at(j).getX(), bussVector.at(j).getY());
			}
		}
		//Boucle pour appliquer la fonction isNotClose à chaque cycliste présent
		for (int k = 0; k < bikesVector.size(); ++k) {
			if (canMove) {
				canMove = car.isNotClose(Moving::BIKE, bikesVector.at(k).getX(), bikesVector.at(k).getY());
			}
		}

		// Boucle pour appliquer la fonction isNotClose à chaque piéton présent
		for (int l = 0; l < pedestriansVector.size(); ++l) {
			if (canMove) {
				canMove = car.isNotClose(Moving::ON_FOOT, pedestriansVector.at(l).getX(), pedestriansVector.at(l).getY());
			}
		}

		// La voiture peut se déplacer uniquement si elle est autorisée par le feu et qu'elle ne vas pas heurter un véhicule ou piéton
		if (canMove) {
			car.move();
			car.turn();
		}

		// Si la voiture quitte la fenêtre, on la fait réapparaître à un autre endroit
		if (car.getX() <= -18.f || car.getX() >= 895.f || car.getY() <= -18.f || car.getY() >= 677.f) {
			cout << "Respawned a car ";
			switch (spawnAndTurnRand(gen)) {
			case 1: spawn = Spawn_area::UP; cout << "at the TOP    "; break;
			case 2: spawn = Spawn_area::DOWN;  cout << "at the BOTTOM "; break;
			case 3: spawn = Spawn_area::LEFT; cout << "to the LEFT   "; break;
			default: spawn = Spawn_area::RIGHT; cout << "to the RIGHT  "; break;
			}
			switch (spawnAndTurnRand(gen)) {
			case 1: turn = Turning::TURN_LEFT; cout << "turning LEFT\n"; break;
			case 2: turn = Turning::TURN_RIGHT; cout << "turning RIGHT\n"; break;
			default: turn = Turning::NO_TURN; cout << "NOT turning\n"; break;
			}
			busLock.lock(); // Mutex lock pour éviter que les véhicules réapparaissent les uns sur les autres
			this_thread::sleep_for(chrono::milliseconds(750));
			car.Respawn(spawn, turn);
			busLock.unlock();
		}

		canMove = true;

		this_thread::sleep_for(chrono::milliseconds(10));
	}

}

// Thread function for moving the bus
void moving_buss(vector<Bus>& bussVector,
	vector<Voiture>& carsVector,
	vector<Cycliste>& bikesVector,
	vector<OnFoot>& pedestriansVector,
	Bus& bus,
	Traffic_light& traffic_light_master,
	Spawn_area spawn,
	chrono::seconds delayMove,
	stop_token stopToken) {

	bool canMove = true;

	this_thread::sleep_for(delayMove);

	while (!stopToken.stop_requested()) {

		// Switch pour check si le bus se trouve dans la zone de détection du feu et si le feu est rouge ou orange. Si les conditions sont remplies, alors la voiture ne pourra pas plus avancer
		switch (spawn) {
		case Spawn_area::LEFT:
			if (bus.getX() <= stopXLeft - 4.f && bus.getX() >= stopXLeft - 10.f && (traffic_light_master.get_traffic_color() == Traffic_color::red || traffic_light_master.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::RIGHT:
			if (bus.getX() >= stopXRight + 4.f && bus.getX() <= stopXRight + 10.f && (traffic_light_master.get_traffic_color() == Traffic_color::red || traffic_light_master.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		}

		// Boucle pour appliquer la fonction isNotClose à chaque bus présent
		for (int j = 0; j < bussVector.size(); ++j) {
			if ((bussVector.at(j).getX() != bus.getX() || bussVector.at(j).getY() != bus.getY()) && canMove) {
				canMove = bus.isNotClose(Moving::BUS, bussVector.at(j).getX(), bussVector.at(j).getY());
			}
		}
		// Boucle pour appliquer la fonction isNotClose à chaque voiture présente
		for (int i = 0; i < carsVector.size(); ++i) {
			if (canMove) {
				canMove = bus.isNotClose(Moving::CAR, carsVector.at(i).getX(), carsVector.at(i).getY());
			}
		}
		//Boucle pour appliquer la fonction isNotClose à chaque cycliste présent
		for (int k = 0; k < bikesVector.size(); ++k) {
			if (canMove) {
				canMove = bus.isNotClose(Moving::BIKE, bikesVector.at(k).getX(), bikesVector.at(k).getY());
			}
		}

		// Boucle pour appliquer la fonction isNotClose à chaque piéton présente
		for (int l = 0; l < pedestriansVector.size(); ++l) {
			if (canMove) {
				canMove = bus.isNotClose(Moving::ON_FOOT, pedestriansVector.at(l).getX(), pedestriansVector.at(l).getY());
			}
		}

		// Le bus peut se déplacer uniquement si il est autorisé par le feu et qu'il ne vas pas heurter un véhicule ou piéton
		if (canMove) {
			bus.move();
		}

		// Si le bus quitte la fenêtre, on le fait réapparaître à un autre endroit
		if (bus.getX() <= -18.f || bus.getX() >= 895.f || bus.getY() <= -18.f || bus.getY() >= 677.f) {
			cout << "Respawned a bus ";
			switch (spawnAndTurnRand(gen)) {
			case 1: spawn = Spawn_area::LEFT; cout << "to the LEFT\n"; break;
			case 2: spawn = Spawn_area::LEFT; cout << "to the LEFT\n"; break;
			default: spawn = Spawn_area::RIGHT; cout << "to the RIGHT\n"; break;
			}
			busLock.lock(); // Mutex lock pour éviter que les véhicules réapparaissent les uns sur les autres
			this_thread::sleep_for(chrono::milliseconds(750));
			bus.Respawn(spawn);
			busLock.unlock();
		}

		canMove = true;

		this_thread::sleep_for(chrono::milliseconds(10));
	}

}

// Thread function for moving the bikes
void moving_bikes(vector<Cycliste>& bikesVector,
	vector<Bus>& bussVector,
	vector<Voiture>& carsVector,
	vector<OnFoot>& pedestriansVector,
	Cycliste& bike,
	Traffic_light& traffic_light_slave,
	Spawn_area spawn,
	chrono::seconds delayMove,
	stop_token stopToken) {

	bool canMove = true;

	this_thread::sleep_for(delayMove);

	while (!stopToken.stop_requested()) {

		// Switch pour check si le cycliste se trouve dans la zone de détection du feu et si le feu est rouge ou orange. Si les conditions sont remplies, alors la voiture ne pourra pas plus avancer
		switch (spawn) {
		case Spawn_area::UP:
			if (bike.getY() <= stopYUp - 4.f && bike.getY() >= stopYUp - 10.f && (traffic_light_slave.get_traffic_color() == Traffic_color::red || traffic_light_slave.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::DOWN:
			if (bike.getY() >= stopYDown + 4.f && bike.getY() <= stopYDown + 10.f && (traffic_light_slave.get_traffic_color() == Traffic_color::red || traffic_light_slave.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
		}

		//Boucle pour appliquer la fonction isNotClose à chaque cycliste présent
		for (int k = 0; k < bikesVector.size(); ++k) {
			if ((bikesVector.at(k).getX() != bike.getX() || bikesVector.at(k).getY() != bike.getY()) && canMove) {
				canMove = bike.isNotClose(bikesVector.at(k).getX(), bikesVector.at(k).getY());
			}
		}
		// Boucle pour appliquer la fonction isNotClose à chaque bus présent
		for (int j = 0; j < bussVector.size(); ++j) {
			if (canMove) {
				canMove = bike.isNotClose(bussVector.at(j).getX(), bussVector.at(j).getY());
			}
		}
		//// Boucle pour appliquer la fonction isNotClose à chaque voiture présente
		for (int i = 0; i < carsVector.size(); ++i) {
			if (canMove) {
				canMove = bike.isNotClose(carsVector.at(i).getX(), carsVector.at(i).getY());
			}
		}

		// Boucle pour appliquer la fonction isNotClose à chaque piéton présente
		for (int l = 0; l < pedestriansVector.size(); ++l) {
			if (canMove) {
				canMove = bike.isNotClose(pedestriansVector.at(l).getX(), pedestriansVector.at(l).getY());
			}
		}

		// Le cycliste peut se déplacer uniquement si il est autorisé par le feu et qu'il ne vas pas heurter un véhicule ou piéton
		if (canMove) {
			bike.move();
		}

		// Si le cycliste quitte la fenêtre, on le fait réapparaître à un autre endroit
		if (bike.getX() <= -18.f || bike.getX() >= 895.f || bike.getY() <= -18.f || bike.getY() >= 677.f) {
			cout << "Respawned a bike ";
			switch (spawnAndTurnRand(gen)) {
			case 1: spawn = Spawn_area::UP; cout << "at the TOP\n"; break;
			case 2: spawn = Spawn_area::UP; cout << "at the TOP\n"; break;
			default: spawn = Spawn_area::DOWN; cout << "at the BOTTOM\n";
			}
			bikeLock.lock(); // Mutex lock pour éviter que les véhicules réapparaissent les uns sur les autres
			this_thread::sleep_for(chrono::milliseconds(750));
			bike.Respawn(spawn);
			bikeLock.unlock();
		}

		canMove = true;

		this_thread::sleep_for(chrono::milliseconds(10));
	}

}

// Thread function for moving the pedestrians
void moving_pedestrians(vector<OnFoot>& pedestriansVector,
	vector<Voiture>& carsVector,
	vector<Bus>& bussVector,
	vector<Cycliste>& bikesVector,
	OnFoot& pedestrian,
	Traffic_light& traffic_light_master,
	Traffic_light& traffic_light_slave,
	Spawn_area spawn,
	Turning turn,
	chrono::seconds delayMove,
	stop_token stopToken) {

	bool canMove = true;

	this_thread::sleep_for(delayMove);

	while (!stopToken.stop_requested()) {

		/* Vérifie si le feu est vert avant de permettre aux voitures de se déplacer
		if (trafficLightSlave.getColor() != TrafficColor::Green &&
			currentX <= stopXRight + 10 && currentX > stopXRight - 10) {
			canMove = false; // Si le feu n'est pas vert et que la voiture est dans la zone d'arrêt, elle doit s'arrêter
		}*/
		switch (spawn) {
		case Spawn_area::UP:
			if (pedestrian.getY() <= stopYUp && pedestrian.getY() >= stopYUp - 6.f && (traffic_light_slave.get_traffic_color() == Traffic_color::red || traffic_light_slave.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::DOWN:
			if (pedestrian.getY() >= stopYDown && pedestrian.getY() <= stopYDown + 6.f && (traffic_light_slave.get_traffic_color() == Traffic_color::red || traffic_light_slave.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::LEFT:
			if (pedestrian.getX() <= stopXLeft && pedestrian.getX() >= stopXLeft - 6.f && (traffic_light_master.get_traffic_color() == Traffic_color::red || traffic_light_master.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		case Spawn_area::RIGHT:
			if (pedestrian.getX() >= stopXRight && pedestrian.getX() <= stopXRight + 6.f && (traffic_light_master.get_traffic_color() == Traffic_color::red || traffic_light_master.get_traffic_color() == Traffic_color::orange)) {
				canMove = false;
			}
			break;
		}




		// Boucle pour appliquer la fonction isNotClose à chaque bus présent
		for (int k = 0; k < pedestriansVector.size(); ++k) {
			if ((pedestriansVector.at(k).getX() != pedestrian.getX() || pedestriansVector.at(k).getY() != pedestrian.getY()) && canMove) {
				canMove = pedestrian.isNotClose(pedestriansVector.at(k).getX(), pedestriansVector.at(k).getY());
			}
		}

		// Boucle pour appliquer la fonction isNotClose à chaque bus présent
		for (int j = 0; j < bussVector.size(); ++j) {
			if (canMove) {
				canMove = pedestrian.isNotClose(bussVector.at(j).getX(), bussVector.at(j).getY());
			}
		}
		//// Boucle pour appliquer la fonction isNotClose à chaque voiture présente
		for (int i = 0; i < carsVector.size(); ++i) {
			if (canMove) {
				canMove = pedestrian.isNotClose(carsVector.at(i).getX(), carsVector.at(i).getY());
			}
		}

		//Boucle pour appliquer la fonction isNotClose à chaque cycliste présent
		for (int l = 0; l < bikesVector.size(); ++l) {
			if (canMove) {
				canMove = pedestrian.isNotClose(bikesVector.at(l).getX(), bikesVector.at(l).getY());
			}
		}

		// La voiture peut se déplacer uniquement si elle est autorisée par le feu et qu'elle ne vas pas heurter un véhicule ou piéton
		if (canMove) {
			pedestrian.move();
			pedestrian.turn();
		}


		// Si la voiture quitte la fenêtre, on la fait réapparaître à un autre endroit
		if (pedestrian.getX() <= -13.f || pedestrian.getX() >= 890.f || pedestrian.getY() <= -13.f || pedestrian.getY() >= 672.f) {
			cout << "Respawned a pedestrian ";
			switch (spawnAndTurnRand(gen)) {
			case 1: spawn = Spawn_area::UP; cout << "at the TOP    "; break;
			case 2: spawn = Spawn_area::DOWN;  cout << "at the BOTTOM "; break;
			case 3: spawn = Spawn_area::LEFT; cout << "to the LEFT   "; break;
			default: spawn = Spawn_area::RIGHT; cout << "to the RIGHT  "; break;
			}
			pedestrianLock.lock(); // Mutex lock pour éviter que les véhicules réapparaissent les uns sur les autres
			this_thread::sleep_for(chrono::milliseconds(750));
			pedestrian.Respawn(spawn, turn);
			pedestrianLock.unlock();
		}

		canMove = true;

		this_thread::sleep_for(chrono::milliseconds(1));
	}

}

int main() {

	stop_source stopping; // Crée stopping de la classe stop_source. Cela permet de générer de requêtes d'arrêts 

	// Listes des véhicules
	vector<Voiture> carsVector;
	vector<Bus> bussVector;
	vector<Cycliste> bikesVector;
	vector<OnFoot> pedestriansVector;

	Turning turn, turn1, turn2, turn3, turn4, turn5, turn6, turn7, turn8, turn9;
	Spawn_area spawn, spawn1, spawn2, spawn3, spawn4, spawn5, spawn6, spawn7, spawn8, spawn9;

	// Horloges pour l'apparition des voitures
	//sf::Clock carClock;
	//int spawnDelay = carDelay(gen);


	Traffic_light traffic_light_master{ Traffic_color::red }; // Crée le feu tricolore maître et esclave et les initialise
	Traffic_light traffic_light_slave{ Traffic_color::red };  // avec la couleur rouge par défaut
	jthread thread_traffic_light_master(run_traffic_light,
		ref(traffic_light_master), ref(traffic_light_slave), stopping.get_token());

	// Charge l'image de la voiture
	sf::Texture imageVoiture;
	if (!imageVoiture.loadFromFile("../../../../img/voiture.png")) {
		cerr << "Erreur : Impossible de charger l'image voiture.png\n";
		return EXIT_FAILURE;
	}
	// Charge l'image du bus
	sf::Texture imageBus;
	if (!imageBus.loadFromFile("../../../../img/bus.png")) {
		cerr << "Erreur : Impossible de charger l'image bus.png\n";
		return EXIT_FAILURE;
	}
	// Charge l'image du cyclsite
	sf::Texture imageCycliste;
	if (!imageCycliste.loadFromFile("../../../../img/velo.png")) {
		cerr << "Erreur : Impossible de charger l'image velo.png\n";
		return EXIT_FAILURE;
	}

	// Charge l'image du piéton
	sf::Texture imagePieton;
	if (!imagePieton.loadFromFile("../../../../img/PersoM.png")) {
		cerr << "Erreur : Impossible de charger l'image PersoM.png\n";
		return EXIT_FAILURE;
	}

	int i = 0;
	for (i = 0; i < 9; ++i) {
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
		case 5: spawn6 = spawn; turn6 = turn; break;
		case 6: spawn7 = spawn; turn7 = turn; break;
		case 7: spawn8 = spawn; turn8 = turn; break;
		case 8: spawn9 = spawn; turn9 = turn;
		}
	}

	Voiture carSingle1(carSpeed, ref(imageVoiture), spawn1, turn1); // Créé une nouvelle voiture
	carsVector.push_back(carSingle1); // Push dans le vecteur
	Voiture carSingle2(carSpeed, ref(imageVoiture), spawn2, turn2); // Créé une nouvelle voiture
	carsVector.push_back(carSingle2); // Push dans le vecteur
	Voiture carSingle3(carSpeed, ref(imageVoiture), spawn3, turn3); // Créé une nouvelle voiture
	carsVector.push_back(carSingle3); // Push dans le vecteur
	Voiture carSingle4(carSpeed, ref(imageVoiture), spawn4, turn4); // Créé une nouvelle voiture
	carsVector.push_back(carSingle4); // Push dans le vecteur
	Voiture carSingle5(carSpeed, ref(imageVoiture), spawn5, turn5); // Créé une nouvelle voiture
	carsVector.push_back(carSingle5); // Push dans le vecteur
	Voiture carSingle6(carSpeed, ref(imageVoiture), spawn6, turn6); // Créé une nouvelle voiture
	carsVector.push_back(carSingle6); // Push dans le vecteur
	Voiture carSingle7(carSpeed, ref(imageVoiture), spawn7, turn7); // Créé une nouvelle voiture
	carsVector.push_back(carSingle7); // Push dans le vecteur
	Voiture carSingle8(carSpeed, ref(imageVoiture), spawn8, turn8); // Créé une nouvelle voiture
	carsVector.push_back(carSingle8); // Push dans le vecteur
	Voiture carSingle9(carSpeed, ref(imageVoiture), spawn9, turn9); // Créé une nouvelle voiture
	carsVector.push_back(carSingle9); // Push dans le vecteur

	auto delayMove = chrono::seconds(0);
	jthread jthread_moving_car1(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(0)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn1, turn1, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car2(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(1)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn2, turn2, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car3(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(2)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn3, turn3, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car4(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(3)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn4, turn4, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car5(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(4)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn5, turn5, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car6(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(5)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn6, turn6, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car7(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(6)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn7, turn7, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car8(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(7)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn8, turn8, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_car9(moving_cars, ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector), ref(carsVector.at(8)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn9, turn9, delayMove, stopping.get_token());

	for (i = 0; i < 4; ++i) {
		cout << "New bus spawned ";
		switch (spawnAndTurnRand(gen)) {
		case 1: spawn = Spawn_area::LEFT; cout << "to the LEFT\n"; break;
		case 2: spawn = Spawn_area::LEFT; cout << "to the LEFT\n"; break;
		default: spawn = Spawn_area::RIGHT; cout << "to the RIGHT\n";
		}
		switch (i) {
		case 0: spawn1 = spawn; break;
		case 1: spawn2 = spawn; break;
		case 2: spawn3 = spawn; break;
		case 3: spawn4 = spawn;
		}
	}

	Bus busSingle1(busSpeed, ref(imageBus), spawn1); // Créé ue nouveau bus
	bussVector.push_back(busSingle1); // Push dans le vecteur
	Bus busSingle2(busSpeed, ref(imageBus), spawn2); // Créé ue nouveau bus
	bussVector.push_back(busSingle2); // Push dans le vecteur
	Bus busSingle3(busSpeed, ref(imageBus), spawn3); // Créé ue nouveau bus
	bussVector.push_back(busSingle3); // Push dans le vecteur
	Bus busSingle4(busSpeed, ref(imageBus), spawn4); // Créé ue nouveau bus
	bussVector.push_back(busSingle4); // Push dans le vecteur

	delayMove = chrono::seconds(0);
	jthread jthread_moving_bus1(moving_buss, ref(bussVector), ref(carsVector), ref(bikesVector), ref(pedestriansVector), ref(bussVector.at(0)), ref(traffic_light_master),
		spawn1, delayMove, stopping.get_token()); delayMove += chrono::seconds(5 / 2);
	jthread jthread_moving_bus2(moving_buss, ref(bussVector), ref(carsVector), ref(bikesVector), ref(pedestriansVector), ref(bussVector.at(1)), ref(traffic_light_master),
		spawn2, delayMove, stopping.get_token()); delayMove += chrono::seconds(5 / 2);
	jthread jthread_moving_bus3(moving_buss, ref(bussVector), ref(carsVector), ref(bikesVector), ref(pedestriansVector), ref(bussVector.at(2)), ref(traffic_light_master),
		spawn3, delayMove, stopping.get_token()); delayMove += chrono::seconds(5 / 2);
	jthread jthread_moving_bus4(moving_buss, ref(bussVector), ref(carsVector), ref(bikesVector), ref(pedestriansVector), ref(bussVector.at(3)), ref(traffic_light_master),
		spawn4, delayMove, stopping.get_token());

	for (i = 0; i < 5; ++i) {
		cout << "New bike spawned ";
		switch (spawnAndTurnRand(gen)) {
		case 1: spawn = Spawn_area::UP; cout << "at the TOP\n"; break;
		case 2: spawn = Spawn_area::UP; cout << "at the TOP\n"; break;
		default: spawn = Spawn_area::DOWN; cout << "at the BOTTOM\n";
		}
		switch (i) {
		case 0: spawn1 = spawn; break;
		case 1: spawn2 = spawn; break;
		case 2: spawn3 = spawn; break;
		case 3: spawn4 = spawn; break;
		case 4: spawn5 = spawn;
		}
	}

	Cycliste bikeSingle1(bikeSpeed, ref(imageCycliste), spawn1); // Créé ue nouveau cycliste
	bikesVector.push_back(bikeSingle1); // Push dans le vecteur
	Cycliste bikeSingle2(bikeSpeed, ref(imageCycliste), spawn2); // Créé ue nouveau cycliste
	bikesVector.push_back(bikeSingle2); // Push dans le vecteur
	Cycliste bikeSingle3(bikeSpeed, ref(imageCycliste), spawn3); // Créé ue nouveau cycliste
	bikesVector.push_back(bikeSingle3); // Push dans le vecteur
	Cycliste bikeSingle4(bikeSpeed, ref(imageCycliste), spawn4); // Créé ue nouveau cycliste
	bikesVector.push_back(bikeSingle4); // Push dans le vecteur
	Cycliste bikeSingle5(bikeSpeed, ref(imageCycliste), spawn5); // Créé ue nouveau cycliste
	bikesVector.push_back(bikeSingle4); // Push dans le vecteur

	delayMove = chrono::seconds(0);
	jthread jthread_moving_bike1(moving_bikes, ref(bikesVector), ref(bussVector), ref(carsVector), ref(pedestriansVector), ref(bikesVector.at(0)), ref(traffic_light_slave),
		spawn1, delayMove, stopping.get_token()); delayMove += chrono::seconds(3);
	jthread jthread_moving_bike2(moving_bikes, ref(bikesVector), ref(bussVector), ref(carsVector), ref(pedestriansVector), ref(bikesVector.at(1)), ref(traffic_light_slave),
		spawn2, delayMove, stopping.get_token()); delayMove += chrono::seconds(3);
	jthread jthread_moving_bike3(moving_bikes, ref(bikesVector), ref(bussVector), ref(carsVector), ref(pedestriansVector), ref(bikesVector.at(2)), ref(traffic_light_slave),
		spawn3, delayMove, stopping.get_token()); delayMove += chrono::seconds(3);
	jthread jthread_moving_bike4(moving_bikes, ref(bikesVector), ref(bussVector), ref(carsVector), ref(pedestriansVector), ref(bikesVector.at(3)), ref(traffic_light_slave),
		spawn4, delayMove, stopping.get_token()); delayMove += chrono::seconds(3);
	jthread jthread_moving_bike5(moving_bikes, ref(bikesVector), ref(bussVector), ref(carsVector), ref(pedestriansVector), ref(bikesVector.at(4)), ref(traffic_light_slave),
		spawn4, delayMove, stopping.get_token());


	for (i = 0; i < 6; ++i) {
		cout << "New pedestrian spawned ";
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

	OnFoot pedestrianSingle1(pedestrianSpeed, ref(imagePieton), spawn1, turn1); // Créé une nouvelle voiture
	pedestriansVector.push_back(pedestrianSingle1); // Push dans le vecteur
	OnFoot pedestrianSingle2(pedestrianSpeed, ref(imagePieton), spawn2, turn2); // Créé une nouvelle voiture
	pedestriansVector.push_back(pedestrianSingle2); // Push dans le vecteur
	OnFoot pedestrianSingle3(pedestrianSpeed, ref(imagePieton), spawn3, turn3); // Créé une nouvelle voiture
	pedestriansVector.push_back(pedestrianSingle3); // Push dans le vecteur
	OnFoot pedestrianSingle4(pedestrianSpeed, ref(imagePieton), spawn4, turn4); // Créé une nouvelle voiture
	pedestriansVector.push_back(pedestrianSingle4); // Push dans le vecteur
	OnFoot pedestrianSingle5(pedestrianSpeed, ref(imagePieton), spawn5, turn5); // Créé une nouvelle voiture
	pedestriansVector.push_back(pedestrianSingle5); // Push dans le vecteur
	OnFoot pedestrianSingle6(pedestrianSpeed, ref(imagePieton), spawn6, turn6); // Créé une nouvelle voiture
	pedestriansVector.push_back(pedestrianSingle6); // Push dans le vecteur


	delayMove = chrono::seconds(0);
	jthread jthread_moving_pedestrian1(moving_pedestrians, ref(pedestriansVector), ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector.at(0)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn1, turn1, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_pedestrian2(moving_pedestrians, ref(pedestriansVector), ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector.at(1)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn2, turn2, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_pedestrian3(moving_pedestrians, ref(pedestriansVector), ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector.at(2)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn3, turn3, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_pedestrian4(moving_pedestrians, ref(pedestriansVector), ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector.at(3)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn4, turn4, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_pedestrian5(moving_pedestrians, ref(pedestriansVector), ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector.at(4)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn5, turn5, delayMove, stopping.get_token()); delayMove += chrono::seconds(3 / 2);
	jthread jthread_moving_pedestrian6(moving_pedestrians, ref(pedestriansVector), ref(carsVector), ref(bussVector), ref(bikesVector), ref(pedestriansVector.at(5)), ref(traffic_light_master), ref(traffic_light_slave),
		spawn6, turn6, delayMove, stopping.get_token());

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

		for (const auto& car : carsVector) {
			window.draw(car.spriteVoiture_);
			//window.draw(car.circleTest);
		}
		for (const auto& bus : bussVector) {
			window.draw(bus.spriteBus_);
			//window.draw(bus.circleTest);
		}
		for (const auto& bike : bikesVector) {
			window.draw(bike.spriteCycliste_);
			//window.draw(bike.circleTest);
		}
		for (const auto& pedestrian : pedestriansVector) {
			window.draw(pedestrian.spritePieton_);
			//window.draw(bus.circleTest);
		}

		window.display(); // Affiche la fenêtre
	}

	return EXIT_SUCCESS;

}