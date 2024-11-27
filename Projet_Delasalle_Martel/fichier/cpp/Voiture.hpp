#ifndef VOITURE_HPP
#define VOITURE_HPP

#include <SFML/Graphics.hpp>
#include <random>

class Voiture {
public:
    sf::Sprite sprite;
    bool decisionTaken = false;
    bool turnRight = false;
    bool turnLeft = false;  

    Voiture(const sf::Texture& texture, bool forward);
    void move(float speed, bool turnRightDirection, bool turnLeftDirection);
    void makeDecision(std::uniform_int_distribution<int>& turnDist, std::mt19937& gen, float x, float y);

private:
    static constexpr float spawnXForward = 3;
    static constexpr float spawnYForward = 339;
    static constexpr float spawnXBackward = 871;
    static constexpr float spawnYBackward = 332;
};

#endif // VOITURE_HPP
