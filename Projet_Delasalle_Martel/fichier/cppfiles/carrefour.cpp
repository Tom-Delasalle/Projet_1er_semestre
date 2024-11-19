#include <iostream>
#include <thread>
#include <SFML/Windows.hpp>
#include <SFML/Graphics.hpp>
#include "carrefour.hpp"

using namespace std;
using namespace chrono_literals; // Permet de faire des op�ration de temps avec s, min, h, ...

static const sf::Color Orange(255, 165, 0); // Cr�e la couleur orange � partir des valeurs RGB

const auto time_transit = 3s;
const auto time_waiting = 8s;

enum class Traffic_color // Enum�ration de couleurs stock� dans la class Traffic_color. Cette d�claration permet d'utiliser directement le nom des couleurs 
{
    green = 0,
    orange = 1,
    red = 2
};

int main() {

	return EXIT_SUCCESS;

}