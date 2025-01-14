#include <iostream>
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

	int n = 24;

	sf::VertexArray lines;
	lines.setPrimitiveType(sf::PrimitiveType::Lines);
	lines.resize(n);

	sf::VertexArray lineStrip;
	lineStrip.setPrimitiveType(sf::PrimitiveType::LineStrip);
	lineStrip.resize(n);

	sf::VertexArray triangles;
	triangles.setPrimitiveType(sf::PrimitiveType::Triangles);
	//triangles.resize(6);
	triangles.resize(width * height * 6);

	float size = 48.0f;

	//sf::Vector2f v0 = { 0.0f, 0.0f };
	//sf::Vector2f v1 = { size, 0.0f };
	//sf::Vector2f v2 = { 0.0f, size };
	//sf::Vector2f v3 = { size,size };
	//
	//triangles[0].position = v0;
	//triangles[1].position = v1;
	//triangles[2].position = v2;
	//triangles[3].position = v3;
	//triangles[4].position = v2;
	//triangles[5].position = v1;

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {

			const int index = 6 * (i * height + j);

			sf::Vector2f v0 = { size * i, size * j };
			sf::Vector2f v1 = { size * i, size * (j + 1)};
			sf::Vector2f v2 = { size * (i + 1), size * j};
			sf::Vector2f v3 = { size * (i + 1),size * (j + 1) };

			triangles[index + 0].position = v0;
			triangles[index + 1].position = v1;
			triangles[index + 2].position = v2;

			triangles[index + 3].position = v3;
			triangles[index + 4].position = v2;
			triangles[index + 5].position = v1;

			triangles[index + 0].color = HSVToRGB(0.0f, 1.0f, 1.0f);
			triangles[index + 1].color = HSVToRGB(60.0f, 1.0f, 1.0f);
			triangles[index + 2].color = HSVToRGB(120.0f, 1.0f, 1.0f);
			triangles[index + 3].color = HSVToRGB(180.0f, 1.0f, 1.0f);
			triangles[index + 4].color = HSVToRGB(240.0f, 1.0f, 1.0f);
			triangles[index + 5].color = HSVToRGB(300.0f, 1.0f, 1.0f);
		}
	}

	for (int i = 0; i < n; ++i) {

		sf::Vector2f pos = { float(i * cos(i)),float(i * sin(i)) };
		float hue = i * (360.0f / n);

		lines[i].position = pos;
		lines[i].position += {width / 4.0f, height / 4.0f};
		lines[i].color = HSVToRGB(hue, 1.0f, 1.0f);

		lineStrip[i].position = pos;
		lineStrip[i].position += {width / 2.0f, height / 2.0f};
		lineStrip[i].color = HSVToRGB(hue, 1.0f, 1.0f);
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
		//Render
		window->clear();

		//Drawing
		window->draw(lines);
		window->draw(lineStrip);
		window->draw(triangles);

		window->display();
	}
	delete window;
	return 0;
}
