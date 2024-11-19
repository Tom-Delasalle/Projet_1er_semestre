#include <iostream>
#include <thread>
#include <SFML/Windows.hpp>
#include <SFML/Graphics.hpp>
#include "carrefour.hpp"

using namespace std;
using namespace chrono_literals; // Permet de faire des opération de temps avec s, min, h, ...

static const sf::Color Orange(255, 165, 0); // Crée la couleur orange à partir des valeurs RGB

const auto time_transit = 3s;
const auto time_waiting = 8s;

enum class Traffic_color // Enumération de couleurs stocké dans la class Traffic_color. Cette déclaration permet d'utiliser directement le nom des couleurs 
{
    green = 0,
    orange = 1,
    red = 2
};

int main() {

	return EXIT_SUCCESS;

}