#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include <atomic>

enum class Spawn_area {
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
};

enum class Turning {
    TURN_LEFT = 1,
    TURN_RIGHT = 2,
    NO_TURN = 3
};

class OnFoot {
private:
    int posX_;
    int posY_;
    float speed_;
    float angle_;
    Spawn_area spawn_;
    Turning turning_;
    sf::Texture imageOnFootM;
    sf::Sprite spriteOnFootM;

    // Thread management
    std::thread movementThread;
    std::atomic<bool> isMoving_;  // Thread-safe flag to control movement

public:
    // Constructor
    OnFoot(const float speed, const Spawn_area& spawn, const Turning& turning);

    // Getters
    int getX();
    int getY();

    // Setters
    void set_speed(const float newSpeed);

    // Movement functions
    void move();
    void moveAutomatically();  // Function to be run in a separate thread
    void stop();  // Stop the pedestrian's movement

    // Thread management functions
    void startMoving();  // Start the movement thread
    void joinThread();   // Join the thread if it's joinable

    // Access to sprite for drawing
    sf::Sprite& getSprite() { return spriteOnFootM; }
};
