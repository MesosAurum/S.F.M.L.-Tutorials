#include <iostream>
#include <string.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

static void PollEvents(sf::RenderWindow &window) {

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

	std::uint16_t width = 640;
	std::uint16_t height = 360;
	sf::VideoMode videoMode = sf::VideoMode({ width, height });
	std::string title = "Mandelbrot set";
	sf::RenderWindow window = sf::RenderWindow(videoMode, title);
	window.setFramerateLimit(60);

	sf::VertexArray vertices;
	vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	vertices.resize(5);

	vertices[0].position = sf::Vector2f(0.0f, 0.0f);
	vertices[1].position = sf::Vector2f(width, 0.0f);
	vertices[2].position = sf::Vector2f(width, height);
	vertices[3].position = sf::Vector2f(0.0f, height);
	vertices[4].position = vertices[0].position;

	sf::Shader shader;
	if(!shader.loadFromFile("Mandelbrot.vert", "Mandelbrot.frag")) {

		std::cerr << "ERROR::COULD_NOT_LOAD_SHADERS" << std::endl;
	}
	sf::Clock clock;
	sf::Time time = clock.restart();

	while(window.isOpen()) {

		//Updating
		PollEvents(window);

		time = clock.getElapsedTime();
		float t = time.asSeconds();

		shader.setUniform("iResolution", sf::Vector2f(width, height));
		shader.setUniform("iTime", t);

		//Rendering
		window.clear();

		window.draw(vertices, &shader);

		window.display();
	}
	return 0;
}
