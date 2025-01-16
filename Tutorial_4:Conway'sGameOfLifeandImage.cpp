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
int main() {

	unsigned int width = 640;
	unsigned int height = 360;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width,height }), "Tutorials");
	window->setFramerateLimit(60);

	sf::Image image;
	image.resize({ width,height });

	sf::Texture texture(image.getSize());

	sf::Sprite sprite(texture);

	unsigned int size = width * height;

	unsigned int *board = new unsigned int[size];
	unsigned int *neighbors = new unsigned int[size];

	int dir[8] = { 1, int(width) + 1, int(width) , int(width) - 1,-1, -int(width) - 1, -int(width) , -int(width) + 1 };

	std::default_random_engine randEng;
	int seed = std::chrono::steady_clock::now().time_since_epoch().count();
	randEng.seed(seed);

	for(unsigned int i = 0; i < size; ++i) {

		std::uniform_int_distribution onOff(0, 1);
		board[i] = onOff(randEng);
		neighbors[i] = 0;
	}

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

		for(unsigned int i = 0; i < size; ++i) {

			if(board[i] == 0) {

				continue;
			}
			for(int j = 0; j < 8; ++j) {

				int index = i + dir[j];
				if(index < 0) {

					index += size;
				}
				else if(index > size - 1) {

					index -= size;
				}
				++neighbors[index];
			}
		}
		for(unsigned int i = 0; i < size; ++i) {

			//If off && neighbors == 3 -> On
			//If on && neighbors < 2 || neighbors > 3 -> Off

			if(board[i] == 0 && neighbors[i] == 3) {

				board[i] = 1;
			}
			else if(board[i] == 1 && (neighbors[i] < 2 || neighbors[i] > 3)) {

				board[i] = 0;
			}

			sf::Color color = board[i] == 1 ? sf::Color::White : sf::Color::Black;

			unsigned int x = i % width;
			unsigned int y = floor(i / width);

			sf::Vector2u pos = sf::Vector2u(x, y);

			image.setPixel(pos, color);

			neighbors[i] = 0;
		}
		texture.update(image);

		//Render
		window->clear();

		//Drawing
		window->draw(sprite);

		window->display();
	}
	delete[] board;
	delete[] neighbors;
	delete window;
	return 0;
}
