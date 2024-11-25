#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tricolore.hpp"

class Voiture {
private:
    sf::RectangleShape shape_;
    sf::Vector2f direction_;
    float speed_;
    bool turn_left_;
    bool turn_right_;
    bool go_straight_;

public:
    Voiture(float x, float y, sf::Vector2f direction, float speed, bool turn_left, bool turn_right, bool go_straight)
        : direction_(direction), speed_(speed), turn_left_(turn_left), turn_right_(turn_right), go_straight_(go_straight) {
        shape_.setSize(sf::Vector2f(20, 10));
        shape_.setPosition(x, y);
        shape_.setFillColor(sf::Color::Blue);
    }

    void update(const std::vector<Voiture>& voitures, const Traffic_light& traffic_light) {
        // Vérifier la couleur du feu tricolore
        if (traffic_light.get_traffic_color() == Traffic_color::red || traffic_light.get_traffic_color() == Traffic_color::orange) {
            // Arrêter la voiture si le feu est rouge ou orange
            return;
        }

        // Vérifier la position des autres voitures
        for (const auto& voiture : voitures) {
            if (&voiture != this && distance(voiture) < 30.0f) {
                // Arrêter la voiture si une autre voiture est trop proche devant
                return;
            }
        }

        // Mettre à jour la position de la voiture
        shape_.move(direction_ * speed_);

        // Vérifier si la voiture doit tourner ou continuer tout droit
        if (turn_left_) {
            // Logique pour tourner à gauche
        }
        else if (turn_right_) {
            // Logique pour tourner à droite
        }
        else if (go_straight_) {
            // Logique pour continuer tout droit
        }
    }

    float distance(const Voiture& other) const {
    }

    bool is_out_of_bounds(const sf::RenderWindow& window) const {
    }
};