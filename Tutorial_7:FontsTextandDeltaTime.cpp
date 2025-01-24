#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <sstream>

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
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width,height }), "Tutorials");
	window->setFramerateLimit(60);

	sf::Font font;
	LoadFont(font, "Fonts/arial.ttf");

	sf::Text text(font);
	text.setString("Time: 0.000000");
	text.setOrigin(text.getGlobalBounds().size / 2.0f);
	text.setPosition({ width / 2.0f, height / 2.0f });
	text.setFillColor(sf::Color(0x6495EDFF));
	text.setOutlineThickness(1.0f);
	text.setOutlineColor(sf::Color(0x9B6A12FF));

	sf::Clock clock;
	float dt = clock.restart().asSeconds();

	while (window->isOpen()) {

		PollEvents(*window);

		dt = clock.restart().asSeconds();

		std::stringstream sStream;
		sStream << "Time: " << dt;
		text.setString(sStream.str());

		//Render
		window->clear();

		//Drawing
		window->draw(text);

		window->display();
	}
	delete window;
	return 0;
}
