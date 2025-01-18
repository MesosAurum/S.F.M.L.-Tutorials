#include <iostream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>

sf::Color HSVToRGB(float h, float s, float v) {

	float hPrime = h / 60.0f;
	unsigned int hIndex = unsigned int(hPrime) % 6;

	float chroma = s * v;
	float min = (v - chroma);
	float x = chroma * (1.0f - abs(fmod(hPrime, 2.0f) - 1.0f));

	float outRGB[6][3] = {

		{chroma,x, 0.0f},
		{x,chroma, 0.0f},
		{0.0f,chroma,x},
		{0.0f,x,chroma},
		{x,0.0f,chroma},
		{chroma,0.0f,x}
	};
	float rF = (outRGB[hIndex][0] + min);
	float gF = (outRGB[hIndex][1] + min);
	float bF = (outRGB[hIndex][2] + min);
	rF *= 255.0f;
	gF *= 255.0f;
	bF *= 255.0f;

	std::uint8_t rI = std::uint8_t(rF);
	std::uint8_t gI = std::uint8_t(gF);
	std::uint8_t bI = std::uint8_t(bF);

	return sf::Color(rI, gI, bI);
}
void PollEvents(sf::RenderWindow &window) {

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

	unsigned int width = 640;
	unsigned int height = 360;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width,height }), "Tutorials");
	window->setFramerateLimit(60);
	sf::RenderWindow *window2 = new sf::RenderWindow(sf::VideoMode({ width,height }), "Tutorials2");
	window2->setFramerateLimit(60);

	sf::CircleShape circle(64.0f);
	circle.setOrigin(circle.getGeometricCenter());
	circle.setPosition({ width / 2.0f,height / 2.0f });
	circle.setFillColor(sf::Color(0x6495EDFF));

	sf::CircleShape circle2(32.0f);
	circle2.setOrigin(circle2.getGeometricCenter());
	circle2.setPosition({ width / 2.0f,height / 2.0f });
	circle2.setFillColor(sf::Color(0x6495EDFF));

	while (window->isOpen()) {

		PollEvents(*window);
		PollEvents(*window2);

		if(window->hasFocus()) {

			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

				sf::Vector2i pos = sf::Mouse::getPosition(*window);
				circle.setPosition(sf::Vector2f(pos));
			}
		}
		if(window2->hasFocus()) {

			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

				sf::Vector2i pos = sf::Mouse::getPosition(*window2);
				circle2.setPosition(sf::Vector2f(pos));
			}
		}
		if(window2->isOpen() == false) {

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::O)) {

				window2 = new sf::RenderWindow(sf::VideoMode({ width,height }), "Tutorials2");
			}
		}

		//Render
		window->clear();
		window2->clear();

		//Drawing
		window->draw(circle);
		window2->draw(circle2);

		window->display();
		window2->display();
	}
	delete window;
	delete window2;
	return 0;
}
