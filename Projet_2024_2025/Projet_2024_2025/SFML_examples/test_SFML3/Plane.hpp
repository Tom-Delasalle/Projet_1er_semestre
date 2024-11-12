#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using namespace std::chrono_literals;


constexpr float incr = static_cast<float>(M_PI / 5.);
constexpr unsigned WINDOW_SIZE_X = 800, WINDOW_SIZE_Y = 800;

constexpr float x_center = WINDOW_SIZE_X / 2.;
constexpr float y_center = WINDOW_SIZE_Y / 2.;

constexpr float initial_rotation = -15 + 90;

class Plane;

void update_plane(Plane &plane, bool &stop_thread);

class Plane
{
    std::thread t_;
    float angle_ = 0;
    float radius_ = 0;
    float speed_ = 0;
    bool stop_thread_ = false;
    float x_ = 0.;
    float y_ = 0.;
    float altitude_ = 0.;
    float omega_ = 0;
    high_resolution_clock::time_point initial_clock_;

public:
    Plane(const float &radius, const float &init_angle, const float &altitude, const float &speed);

    void update();
    float get_x();

    float get_y();

    float get_angle();

    ~Plane();
};
