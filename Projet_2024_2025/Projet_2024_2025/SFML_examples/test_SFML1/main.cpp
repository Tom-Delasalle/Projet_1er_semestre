#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Leader.hpp"
#include "Follower.hpp"

using namespace std;
using namespace sf;


int main() {
	// Fenêtre
	const Vector2u WINDOW_SIZE(800, 600);
	
	RenderWindow app(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y, 32), "My Camera");

	// Frames Per Second (FPS)
	app.setFramerateLimit(60); // limite la fenêtre à 60 images par seconde

	Texture backgroundImage;
	Sprite backgroundSprite;

	if (!backgroundImage.loadFromFile(path_image+"background.png")) {
	 	cerr << "Cannot load image file : " << path_image + "background.png" << endl;
	 	return EXIT_FAILURE; // On ferme le programme
	}
	
	backgroundSprite.setTexture(backgroundImage);



	sf::IntRect rect_runner;


	int runner_initial_pos_x = 204;
 	int runner_final_pos_x = 1024;
	int runner_incr_x = 102;

	Animation_setting animation_runner = {204, 1024, 102 };

	Follower runner(path_image+"runner.png", animation_runner, 50, 10,	102, 115, 0.3f);
/********************************/
/********************************/
	sf::IntRect rect_circle;

	int circle_initial_pos_x = 0;
 	int circle_final_pos_x = 7500;
	int circle_incr_x = 300;

	Animation_setting animation_circle = {0, 7500, 300};

	rect_circle.top = 0;
	rect_circle.left = 0;

	rect_circle.width = 300;
	rect_circle.height = 300;

	constexpr int shift_x_circle = 100;
	constexpr int shift_y_circle = 10;

	Follower circle(path_image+"sprites_final.png", animation_circle,
	100, 10, 300, 300, 0.15f);

/********************************/

	Leader car(path_image+"car.png", 790, 1215);

	car.add_followers(runner);
	car.add_followers(circle);

	Vector2f center(car.getX(), car.getY());

	Vector2f halfSize(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y / 2.f);

	View view(center, halfSize);

	app.setView(view);
	
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

		car.move();
		view.setCenter(car.getX(), car.getY());

		app.setView(view);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{
			car.turnLeft();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			car.turnRight();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			car.speedUp();
		}
		else
		{
			car.speedDown();
		}

		// Affichages
		app.clear();
		app.draw(backgroundSprite);

		car.draw(app);

		app.display();
	}

	return EXIT_SUCCESS;
}
