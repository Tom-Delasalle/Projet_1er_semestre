#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Leader.hpp"

Leader::Leader(const std::string &filename, const float &x, const float &y,
               const float &scale)
    : x_(x), y_(y), angle_(0), speed_(0)
{
    if (!image_.loadFromFile(filename))
    {
        std::cerr << "Cannot load picture : " << filename << std::endl;
        exit(EXIT_FAILURE); // On ferme le programme
    }
    sprite_.setTexture(image_);
    sprite_.setScale(sf::Vector2f(scale, scale));
}

float Leader::getX() const
{
    return x_;
}

float Leader::getY() const
{
    return y_;
}

float Leader::getAngle() const
{
    return angle_;
}

void Leader::move()
{
    x_ += static_cast<float>(cos(angle_ * M_PI / 180.0) * speed_);
    y_ += static_cast<float>(sin(angle_ * M_PI / 180.0) * speed_);
    sprite_.setPosition(this->getX(), this->getY());
        for (auto &ifol : followers)
    {
        ifol.move(x_, y_, angle_);
    }

}

void Leader::turn(){
    sprite_.setRotation(angle_);
    for (auto &ifol : followers)
    {
        ifol.turn(angle_);
    }
}

void Leader::turnLeft()
{
    angle_ += 1;
    turn();
}

void Leader::turnRight()
{
    angle_ -= 1;
    turn();
}

void Leader::speedUp()
{
    if (speed_ < 3.f)
        speed_ += 0.05f;

    for (auto &ifol : followers)
    {
        ifol.speedUp();
    }
}

void Leader::speedDown()
{
    if (speed_ > 0.f)
        speed_ -= 0.05f;
    else
        speed_ = 0;

    for (auto &ifol : followers)
    {
        ifol.speedDown();
    }
}

void Leader::add_followers(Follower &follower) {
    followers.push_back(follower);
}

void Leader::draw(sf::RenderWindow &app)
{
    app.draw(sprite_);
    for (auto &ifol : followers)
    {
        app.draw(ifol.get_sprite());
    }
}
