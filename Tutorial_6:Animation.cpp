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

	sf::Texture texture;

	if(!texture.loadFromFile("Sprites/AnimationExample.png")) {

		std::cerr << "ERROR COULD NOT LOAD FILE::Sprites/AnimationExample.png!!!" << std::endl;
		return -1;
	}

	sf::Sprite sprite(texture);

	int texWidth = 0;

	sprite.setTextureRect({ {0,0}, {32,32} });
	sprite.setOrigin({ sprite.getTextureRect().size.x / 2.0f, sprite.getTextureRect().size.y / 2.0f });
	sprite.setPosition({ width / 2.0f, height / 2.0f });
	sprite.setScale({ 4.0f,4.0f });

	float timer = 0.0f;
	float timerMax = 0.25f;
	float waitTimerMax = 2.25f;
	float waitTimer = waitTimerMax;

	while (window->isOpen()) {

		PollEvents(*window);

		if(waitTimer >= waitTimerMax) {

			timer += 0.1f;
			if(timer >= timerMax) {

				texWidth += 32;

				if(texWidth >= texture.getSize().x) {

					texWidth = 0;
				}
				if(texWidth < texture.getSize().x) {

					sprite.setTextureRect({ {texWidth,0}, {32,32} });
				}

				timer = 0.0f;
			}
			if(texWidth == texture.getSize().x / 2.0f) {

				waitTimer = 0.0f;
			}
		}
		if(waitTimer < waitTimerMax) {

			waitTimer += 0.1f;
		}
		

		//Render
		window->clear();

		//Drawing
		window->draw(sprite);

		window->display();
	}
	delete window;
	return 0;
}
