#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Plane.hpp"

using namespace std;
using namespace sf;

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _PATH_IMG_ "C:/Program Files/SFML/img/"
#else
#define _PATH_IMG_ "../img/"
#endif

const std::string path_image(_PATH_IMG_);

struct Plane_with_sprite : public Plane
{
	//	Plane plane_;
	sf::Sprite sprite_;
	Plane_with_sprite(const float &radius, const float &angle_init, const float &altitude, const float &speed,
					  sf::Texture &texture) : Plane(radius, -angle_init, altitude, speed)
	{
		sprite_.setTexture(texture);
		auto scale = .1f;
		sprite_.setScale(sf::Vector2f(scale, scale));
	}
	sf::Sprite &update()
	{
		sprite_.setPosition(get_x(), get_y());
		sprite_.setRotation(get_angle());
		return sprite_;
	}
};

void visualization(std::vector<unique_ptr<Plane_with_sprite>> &planes)
{
	// Fenêtre
	RenderWindow app(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32), "My Camera");

	// Frames Per Second (FPS)
	app.setFramerateLimit(30); // limite la fenêtre à 60 images par seconde

	Texture backgroundImage;
	Sprite backgroundSprite;

	// if (!backgroundImage.loadFromFile(path_image + "background.png"))
	// {
	// 	cerr << "Cannot load image file : " << path_image + "background.png" << endl;
	// 	return EXIT_FAILURE; // On ferme le programme
	// }

	backgroundImage.create(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	backgroundSprite.setTexture(backgroundImage);

	/********************************/


	while (app.isOpen()) // Boucle principale
	{
		Event event;
		while (app.pollEvent(event)) // Boucle des évènements de la partie pause
		{
			if ((event.type == Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ||
				event.type == Event::Closed)
			{
				app.close();
			}
		}

		// Affichages
		app.clear();
		app.draw(backgroundSprite);

		for (auto &plane : planes)
		{
			app.draw(plane->update());
		}

		app.display();
	}
}

int main(){
		std::vector<unique_ptr<Plane_with_sprite>> planes;
	constexpr float speed = 10;
	Texture plane_image;

	auto filename = path_image + "Daco_5572966.png";

	if (!plane_image.loadFromFile(filename))
	{
		std::cerr << "Cannot load picture : " << filename << std::endl;
		exit(EXIT_FAILURE); // On ferme le programme
	}
	
	planes.push_back(make_unique<Plane_with_sprite>(100.f, 0.f, 1000.f, speed, plane_image));
	planes.push_back(make_unique<Plane_with_sprite>(200.f, 45.f, 1000.f, speed * 2, plane_image));
	planes.push_back(make_unique<Plane_with_sprite>(300.f, 90.f, 1100.f, speed * 3, plane_image));
	planes.push_back(make_unique<Plane_with_sprite>(400.f, 135.f, 1200.f, speed * 4, plane_image));
	
	std::thread UI(visualization, std::ref(planes));

	int i = 1;
	cin >> i;
	if (i) {
		planes.push_back(make_unique<Plane_with_sprite>(50.f, 180.f, 1400.f, speed * 4, plane_image));
	}

	UI.join();
}
