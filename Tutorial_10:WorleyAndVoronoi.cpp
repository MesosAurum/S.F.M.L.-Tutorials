#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/VertexArray.hpp>
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

	sf::Clock clock;
	sf::Time time = clock.restart();

	unsigned int w = 640;
	unsigned int h = 360;
	sf::Vector2u windowSize = { w, h };
	sf::VideoMode videoMode = sf::VideoMode(windowSize);
	sf::RenderWindow window{ videoMode,"Worley and Voronoi" };
	window.setFramerateLimit(60);

	float wFloat = float(w);
	float hFloat = float(h);

	sf::VertexArray verts;
	verts.setPrimitiveType(sf::PrimitiveType::TriangleFan);
	verts.resize(5);
	verts[0].position = { 0.0f, 0.0f };
	verts[1].position = { wFloat, 0.0f };
	verts[2].position = { wFloat, hFloat };
	verts[3].position = { 0.0f, hFloat };
	verts[4].position = verts[0].position;

	sf::Shader shader;
	if(!shader.loadFromFile("Voronoi.vert", "Voronoi.frag")) {

		std::cerr << "ERROR!!!::COULD_NOT_LOAD_SHADERS!!!" << std::endl;
	}

	while(window.isOpen()) {

		time = clock.getElapsedTime();

		//Update
		pollEvents(window);

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
		mousePos = mousePos.componentWiseDiv({ wFloat, hFloat });

		shader.setUniform("iResolution", sf::Vector2f(wFloat, hFloat));
		shader.setUniform("iTime", time.asSeconds());

		//Render section
		//Clearing the screen.
		window.clear();

		window.draw(verts, &shader);

		//Displaying what's drawn.
		window.display();
	}
	return 0;
}
