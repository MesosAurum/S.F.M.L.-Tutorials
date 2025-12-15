#include <iostream>
#include <random>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

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
	std::string title = "Circle collision";
	sf::RenderWindow window = sf::RenderWindow(videoMode, title);
	window.setFramerateLimit(60);

	//Setting up a random number generator.
	std::default_random_engine randomEngine;
	randomEngine.seed(int(std::chrono::steady_clock::now().time_since_epoch().count()));
	//Random float for random position and velocities and positions of our circles.
	std::uniform_real_distribution<float> randomFloat;
	//Random char for random colors of our circles.
	std::uniform_int_distribution<unsigned int> randomChar;
	randomChar = std::uniform_int_distribution<unsigned int>(0, 255);

	//Setting up our circles and the velocities of those circles.
	const unsigned int numCircles = 10;
	sf::CircleShape circles[numCircles];
	sf::Vector2f velocities[numCircles];

	for(unsigned int i = 0; i < numCircles; ++i) {

		//Setting the circle's radius to a random number between 7 and 24.
		randomFloat = std::uniform_real_distribution<float>(7.0f, 24.0f);
		float radius = randomFloat(randomEngine);
		circles[i].setRadius(radius);

		//Random x and y for this circle's position.
		randomFloat = std::uniform_real_distribution<float>(radius, float(windowSize.x) - 2.0f * radius);
		float x = randomFloat(randomEngine);
		randomFloat = std::uniform_real_distribution<float>(radius, float(windowSize.y) - 2.0f * radius);
		float y = randomFloat(randomEngine);
		circles[i].setOrigin(circles[i].getGeometricCenter());
		circles[i].setPosition({ x, y });
		
		//For the color of this circles.
		sf::Color color;
		std::uint8_t r = randomChar(randomEngine);
		std::uint8_t g = randomChar(randomEngine);
		std::uint8_t b = randomChar(randomEngine);
		color = sf::Color(r, g, b);
		circles[i].setFillColor(color);

		//For the starting velocity of the circles.
		randomFloat = std::uniform_real_distribution<float>(-2.0f, 2.0f);
		//Random change in x (displacement in this case) to be used for this circle's velocity.
		sf::Vector2f vel = { randomFloat(randomEngine), randomFloat(randomEngine) };
		velocities[i] = vel;
	}

	while(window.isOpen()) {

		//Update
		pollEvents(window);

		for(unsigned int i = 0; i < numCircles; ++i) {

			//Move the circle at its given velocity.
			circles[i].move(velocities[i]);
			//Getting the position of this circle.
			sf::Vector2f position0 = circles[i].getPosition();
			//Getting the radius of this circle.
			float radius0 = circles[i].getRadius();
			//If the circle is at the border or out of bounds, reverse its velocity.
			if(position0.x < radius0 ||
				position0.x + radius0 > float(windowSize.x)) {

				velocities[i].x = -velocities[i].x;
			}
			if(position0.y < radius0 ||
				position0.y + radius0 > float(windowSize.y)) {

				velocities[i].y = -velocities[i].y;
			}
			//Looping through all the other circles.
			for(unsigned int j = 0; j < numCircles; ++j) {

				//We don't want to check this(i) circle against itself.
				if(i == j) {

					continue;
				}
				//Getting that(j) circle's position and radius.
				sf::Vector2f position1 = circles[j].getPosition();
				float radius1 = circles[j].getRadius();

				//Checking the difference in the position of the circles.
				sf::Vector2f dx = position1 - position0;
				//Getting the distance of the circle.
				float dist = sqrt(dx.x * dx.x + dx.y * dx.y);

				//If the distance between the circles' centers is less than their combined radius.
				if(dist <= radius0 + radius1) {

					float distSq = dist * dist;
					//We get the difference in the circles' velocities.
					sf::Vector2f dv = velocities[j] - velocities[i];

					//Elastic collision equations:
					//v_0f = v_0 - (2 * m_1) / (m_0 + m_1) * dot(-d_v, -d_x) / dist(d_x.x^2 + d_x.y^2) * -d_x / dist(d_x.x^2 + d_x.y^2).
					//v_1f = v_1 - (2 * m_0) / (m_0 + m_1) * dot(d_v, d_x) / dist(d_x.x^2 + d_x.y^2) * d_x / dist(d_x.x^2 + d_x.y^2).
					//All masses here are equal to 1 so we simplify:
					//v_0f = v_0 - dot(-d_v, -d_x) / sqrt(d_x.x^2 + d_x.y^2)^2 * -d_x.
					//v_1f = v_1 - dot(d_v, d_x) / sqrt(d_x.x^2 + d_x.y^2)^2 * d_x.
					sf::Vector2f newVel0 = velocities[i] - (-dv.dot(-dx)) / distSq * -dx;
					sf::Vector2f newVel1 = velocities[j] - (dv.dot(dx)) / distSq * dx;
					velocities[i] = newVel0;
					velocities[j] = newVel1;
				}
			}
		}

		//Render section
		//Clearing the screen.
		window.clear();

		//Drawing all the circles.
		for(unsigned int i = 0; i < numCircles; ++i) {

			window.draw(circles[i]);
		}

		//Displaying what's drawn.
		window.display();
	}
	return 0;
}
