#include <iostream>
#include <SFML/Graphics.hpp>

int main(){

	unsigned int width = 640;
	unsigned int height = 360;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width,height }), "Test");
	window->setFramerateLimit(0x3C);

	sf::CircleShape circle(64.0f);
	circle.setOrigin(circle.getGeometricCenter());
	circle.setPosition({ width / 4.0f, height / 4.0f });
	circle.setFillColor(sf::Color::Green);
	circle.setOutlineThickness(3.0f);
	circle.setOutlineColor(sf::Color::Magenta);
	circle.setPointCount(3);

	sf::RectangleShape rectangle({ 50.0f,80.0f });
	rectangle.setOrigin(rectangle.getSize() / 2.0f);
	rectangle.setPosition({ width / 2.0f, height / 2.0f });
	rectangle.setFillColor(sf::Color::Yellow);
	rectangle.setOutlineThickness(5.0f);
	rectangle.setOutlineColor(sf::Color::Blue);

	sf::ConvexShape convex;
	convex.setPointCount(6);
	convex.setPoint(0, { 13.0f,17.0f });
	convex.setPoint(1, { 3.5f,1.6f });
	convex.setPoint(2, { 0.25f,-12.0f });
	convex.setPoint(3, { -12.0f,-7.3f });
	convex.setPoint(4, { -12.5f,-1.6f });
	convex.setPoint(5, { -5.0f,7.5f });
	convex.setOrigin(convex.getGeometricCenter());
	convex.setFillColor(sf::Color(0x3F00FFFF));
	convex.setOutlineThickness(2.4);
	convex.setOutlineColor(sf::Color(0xFF8888FF));
	convex.setPosition({ width / 1.5f, height / 2.0f });

	while (window->isOpen()) {

		while (const std::optional event = window->pollEvent()) {

			if (event->is<sf::Event::Closed>()) {

				window->close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {

					window->close();
				}
			}
		}
		circle.rotate(sf::degrees(1));
		circle.move({ 1.0f,1.0f });
		rectangle.move({ -1.0f,-1.0f });

		circle.setFillColor(sf::Color::Green);
		rectangle.setFillColor(sf::Color::Yellow);

		if (circle.getGlobalBounds().findIntersection(rectangle.getGlobalBounds())) {

			circle.setFillColor(sf::Color::Red);
			rectangle.setFillColor(sf::Color::Red);
		}

		//Render
		window->clear();

		//Drawing
		window->draw(circle);
		window->draw(rectangle);
		window->draw(convex);

		window->display();
	}
	delete window;
	return 0;
}
