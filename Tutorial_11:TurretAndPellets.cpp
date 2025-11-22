#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

void pollEvents(sf::RenderWindow &window) {

	while(const std::optional event = window.pollEvent()) {

		if(event->is<sf::Event::Closed>()) {

			window.close();
		}
		else if(const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {

			if(keyPressed->scancode == sf::Keyboard::Scancode::Escape) {

				window.close();
			}
		}
	}
}
int main() {

	//Setting up the window.
	unsigned int width = 640;
	unsigned int height = 360;
	sf::Vector2u windowSize = { width, height };
	sf::VideoMode videoMode = sf::VideoMode(windowSize);
	std::string title = "Turret and pellets";
	sf::RenderWindow window = sf::RenderWindow(videoMode, title);
	window.setFramerateLimit(60);

	//Setting up a clock for delta time to use later.
	sf::Clock clock;
	sf::Time time = clock.restart();

	//This will be our stationary turret that our pellets will launch from.
	sf::CircleShape turret(24.0f);
	turret.setOrigin(turret.getGeometricCenter());
	turret.setPointCount(3);
	turret.setPosition(0.5f * sf::Vector2f(windowSize));
	turret.setFillColor(sf::Color::Green);

	//Pellets that will launch toward our mouse position.
	const unsigned int numPellets = 10;
	float speed = 5.0f;
	sf::Vector2f velocities[numPellets];
	sf::CircleShape pellets[numPellets];

	//Setting up our pellets and velocities.
	for(unsigned int i = 0; i < numPellets; ++i) {

		pellets[i].setRadius(2.0f);
		pellets[i].setOrigin(pellets[i].getGeometricCenter());
		pellets[i].setPosition(-sf::Vector2f(pellets[i].getRadius(), pellets[i].getRadius()));
		pellets[i].setFillColor(sf::Color::Red);

		velocities[i] = { 0.0f, 0.0f };
	}
	//Which pellet we are launching.
	unsigned int pelletIndex = 0;

	//Timers for when we launch pellets.
	float pelletTimer = 0.0f;
	float pelletTimerMax = 0.25f;

	//acos(0) = 1.57079632679...
	const float halfPi = acos(0.0f);

	while(window.isOpen()) {

		//Update
		pollEvents(window);

		//Getting delta time.
		time = clock.restart();
		float dt = time.asSeconds();
		pelletTimer += dt;

		//Getting where our mouse is.
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

		//Getting the rotation of our turret toward our mouse.
		float dx = mousePos.x - turret.getPosition().x;
		float dy = mousePos.y - turret.getPosition().y;
		float rotation = atan2(dy, dx);
		turret.setRotation(sf::radians(rotation + halfPi));

		//When our timer reaches its max amount
		//and our mouse is being clicked then we proceed to this block.
		if(pelletTimer >= pelletTimerMax && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

			//Where we place our pellet and set this specific pellet's velocity.
			float theta = turret.getRotation().asRadians();
			theta -= halfPi;
			sf::Vector2f dir = sf::Vector2f(cos(theta), sin(theta));
			velocities[pelletIndex] = speed * dir;
			dir *= turret.getRadius();
			pellets[pelletIndex].setPosition(turret.getPosition() + dir);
			//In the next loop the next pellet in the array may be called.
			++pelletIndex;

			//If we reached the end of the arrays we go back to the first index.
			if(pelletIndex >= numPellets) {

				pelletIndex = 0;
			}
			//Resetting the timer.
			pelletTimer = 0.0f;
		}
		//Moving all the pellets.
		for(unsigned int i = 0; i < numPellets; ++i) {

			pellets[i].move(velocities[i]);
		}

		//Render section
		//Clearing the screen.
		window.clear();

		window.draw(turret);
		//Drawing all the pellets.
		for(unsigned int i = 0; i < numPellets; ++i) {

			window.draw(pellets[i]);
		}

		//Displaying what's drawn.
		window.display();
	}
	return 0;
}
