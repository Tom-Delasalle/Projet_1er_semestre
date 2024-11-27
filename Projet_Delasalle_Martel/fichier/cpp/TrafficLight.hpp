#ifndef TRAFFICLIGHT_HPP
#define TRAFFICLIGHT_HPP

#include <SFML/Graphics.hpp>

enum class TrafficColor { Green, Orange, Red };

class TrafficLight {
public:
    TrafficLight(TrafficColor color);
    void update();
    const sf::Color& getSfmlColor() const;
    TrafficColor getColor() const;

private:
    TrafficColor currentColor;
};

#endif // TRAFFICLIGHT_HPP
