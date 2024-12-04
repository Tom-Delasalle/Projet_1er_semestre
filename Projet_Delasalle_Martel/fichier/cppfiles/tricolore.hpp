#pragma once

#include <chrono>
#include <thread>

using namespace std;

const auto time_transit = 3s;
const auto time_waiting = 8s;

enum class Traffic_color // Enumération de couleurs stocké dans la class Traffic_color. Cette déclaration permet d'utiliser directement le nom des couleurs 
{
    green = 0,
    orange = 1,
    red = 2
};

static Traffic_color operator++(Traffic_color& traffic_color) // Surcharge de l'opérateur ++ pour la class Traffic_color
{
    switch (traffic_color)
    {
    case Traffic_color::red: // Si la couleur actuelle est rouge, elle devient verte
        traffic_color = Traffic_color::green;
        break;

    case Traffic_color::green: // Si la couleur actelle est verte
        this_thread::sleep_for(time_transit); // Attendre 3 secondes
        traffic_color = Traffic_color::orange; // La couleur devient orange
        this_thread::sleep_for(time_transit); // Attendre 3 secondes
        traffic_color = Traffic_color::red; // La couleur devient rouge
    }
    return traffic_color; // On retourne la couleur actuelle
}

class Traffic_light // Création de la class Traffic_light
{
private:
    Traffic_color traffic_color_; // variable correspondant à une énumération de couleurs : vert, orange et rouge

public:
    explicit Traffic_light(const Traffic_color& traffic_color) : traffic_color_{ traffic_color } {} // Constructeur qui prend un Traffic_color en paramètre
    void operator++() // Surcharge de l'opérateur ++ pour la classe Traffic_light 
    {
        ++traffic_color_;
    }
    void set_traffic_color(const Traffic_color& traffic_color) // Fonction qui renvoie
    {
        traffic_color_ = traffic_color;
    }
    const Traffic_color& get_traffic_color() const // Fonction qui renvoie la couleur actuelle du feu tricolore
    {
        return traffic_color_;
    }
};