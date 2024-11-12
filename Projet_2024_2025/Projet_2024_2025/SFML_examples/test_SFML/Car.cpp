#include "Car.hpp"

Car::Car(float x, float y, float angle, float speed)
: _x(x), _y(y), _angle(angle), _speed(speed)
{}

float
Car::getX() const
{
    return _x;
}

float
Car::getY() const
{
    return _y;
}

float
Car::getAngle() const
{
    return _angle;
}

void
Car::move()
{
    _x += static_cast<float>(cos(_angle * M_PI / 180.0) * _speed);
    _y += static_cast<float>(sin(_angle * M_PI / 180.0) * _speed);
}

void
Car::turnLeft()
{
    _angle += 1.f;
}

void
Car::turnRight()
{
    _angle -= 1.f;
}

void
Car::speedUp()
{
    if(_speed < 3.f)
        _speed += 0.05f;
}

void
Car::speedDown()
{
    if (_speed > 0.f)
        _speed -= 0.05f;
    else
        _speed = 0;
}