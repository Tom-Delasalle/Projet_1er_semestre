#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Follower.hpp"

Follower::Follower(const std::string &filename,
                   const Animation_setting &animation_setting,
                   const int &shift_x_from_leader, const int &shift_y_from_leader,
                   const int &width, const int &height,
                   const float &scale) : shift_x_from_leader_(shift_x_from_leader),
                                         shift_y_from_leader_(shift_y_from_leader),
                                         animation_setting_(animation_setting)
{
    rect_.top = 0;
    rect_.left = 0;
    rect_.width = width;
    rect_.height = height;

    if (!image_.loadFromFile(filename))
    {
        std::cerr << "Cannot load picture : " << path_image + filename << std::endl;
        exit(EXIT_FAILURE); // On ferme le programme
    }
    sprite_.setTexture(image_);
    sprite_.setTextureRect(rect_);
    sprite_.setScale(sf::Vector2f(scale, scale));
}

void Follower::speedUp()
{
    if (clock.getElapsedTime().asSeconds() > 0.08)
    {
        rect_.left += animation_setting_.incr_x_;

        if (rect_.left >= animation_setting_.final_pos_x_)
        {
            rect_.left = animation_setting_.initial_pos_x_;
        }
        clock.restart();
    }
}

void Follower::speedDown()
{
    rect_.top = 0;
    rect_.left = 0;
}

float Follower::get_dx(const float &angle) const
{
    float dx =
        static_cast<float>(cos(M_PI + angle) * shift_x_from_leader_ +
                           sin(M_PI + angle) * shift_y_from_leader_);
    return dx;
}

float Follower::get_dy(const float &angle) const
{
    float dy = static_cast<float>(sin(M_PI + angle) * shift_x_from_leader_ +
                                  cos(M_PI + angle) * shift_y_from_leader_);
    return dy;
}

const sf::Sprite &Follower::get_sprite() const
{
    return sprite_;
}

void Follower::turn(const float &angle)
{
    sprite_.setRotation(angle);
}

void Follower::move(const float &x, const float &y, const float &angle)
{
    float local_angle = static_cast<float>(angle * M_PI / 180.0f);

    sprite_.setPosition(x + get_dx(local_angle), y + get_dy(local_angle));
    sprite_.setTextureRect(rect_);
}
