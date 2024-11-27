#include "TrafficLight.hpp"

static const sf::Color Orange(255, 165, 0);

TrafficLight::TrafficLight(TrafficColor color) : currentColor(color) {}

void TrafficLight::update() {
    switch (currentColor) {
    case TrafficColor::Green:
        currentColor = TrafficColor::Orange;
        break;
    case TrafficColor::Orange:
        currentColor = TrafficColor::Red;
        break;
    case TrafficColor::Red:
        currentColor = TrafficColor::Green;
        break;
    }
}

const sf::Color& TrafficLight::getSfmlColor() const {
    switch (currentColor) {
    case TrafficColor::Green:
        return sf::Color::Green;
    case TrafficColor::Orange:
        return Orange;
    case TrafficColor::Red:
        return sf::Color::Red;
    }
    return sf::Color::Black; // Défaut, ne devrait jamais arriver
}

TrafficColor TrafficLight::getColor() const {
    return currentColor;
}
