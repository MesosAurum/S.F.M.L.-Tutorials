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
void LoadFont(sf::Font &font, std::string str) {

	if(!font.openFromFile(str)) {

		std::cerr << "ERROR::COULD NOT LOAD FILE::" << str << "!!!" << std::endl;
	}
}
int main() {

	unsigned int width = 640;
	unsigned int height = 360;
	sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode({ width,height }), "Tutorials");
	window->setFramerateLimit(60);
	sf::RenderWindow *window2 = new sf::RenderWindow(sf::VideoMode({ width / 3,height / 2 }), "Tutorials2");
	window2->setFramerateLimit(60);

	window2->setPosition(window->getPosition());

	while(window->isOpen()) {

		//Update
		window2->setPosition(window->getPosition());

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::O) && window2->isOpen() == false) {

			window2 = new sf::RenderWindow(sf::VideoMode({ width / 3,height / 2 }), "Tutorials2");
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

			window2->close();
		}
		std::cout << "X: " << sf::Mouse::getPosition(*window).x << std::endl;
		std::cout << "Y: " << sf::Mouse::getPosition(*window).y << std::endl << std::endl;

		PollEvents(*window);
		PollEvents(*window2);

		//Render
		window->clear();
		window2->clear();

		//Drawing

		window->display();
		window2->display();
	}
	delete window;
	delete window2;
	return 0;
}
