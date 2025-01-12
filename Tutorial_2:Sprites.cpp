#include <iostream>
#include <SFML/Graphics.hpp>

enum directions { down, right, up, left };
int main() {

	unsigned int width = 640;
	unsigned int height = 360;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width,height }), "Tutorials");
	window->setFramerateLimit(60);

	sf::Texture texture;

	if (!texture.loadFromFile("Sprites/ExampleSprite.png")) {

		std::cerr << "ERROR::COULD NOT LOAD FILE::Sprites/ExampleSprite.png" << std::endl;
		return -1;
	}
	sf::Sprite sprite(texture);

	sf::IntRect dir[4];

	for (int i = 0; i < 4; ++i) {

		dir[i] = sf::IntRect({ {32 * i, 0}, {32,32} });
	}
	sprite.setTextureRect(dir[down]);
	sprite.setOrigin({ 16,16 });
	sprite.setPosition({ width / 2.0f, height / 2.0f });
	sprite.setColor(sf::Color(0x6495EDFF));

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
		sprite.rotate(sf::degrees(1));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {

			sprite.move({ 0.0f, 1.0f });
			sprite.setTextureRect(dir[down]);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {

			sprite.move({ 0.0f, -1.0f });
			sprite.setTextureRect(dir[up]);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {

			sprite.move({ 1.0f, 0.0f });
			sprite.setTextureRect(dir[right]);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {

			sprite.move({ -1.0f, 0.0f });
			sprite.setTextureRect(dir[left]);
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
