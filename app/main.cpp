#include "Canvas.h"
#include "Brush.h"
#include "Camera.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	const sf::Color clearColor = sf::Color(255, 0, 255);
	const unsigned int windowWidth = 1600;
	const unsigned int windowHeight = 900;

	sf::RenderWindow window(
		sf::VideoMode(windowWidth, windowHeight, 32),
		"Pixel Pastel",
		sf::Style::Titlebar | sf::Style::Close);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	Canvas canvas(sf::Vector2u(32, 32));
	Brush brush;
	Camera camera;

	const sf::Vector2u &canvasSize = canvas.getSize();

	camera.setZoom(
		std::fminf(
			windowWidth / static_cast<float>(canvasSize.x),
			windowHeight / static_cast<float>(canvasSize.y)) * 0.75f);

	camera.setPosition(
		sf::Vector2f(
			(canvasSize.x - windowWidth / camera.getZoom()) * 0.5f,
			(canvasSize.y - windowHeight / camera.getZoom()) * 0.5f));

	while (window.isOpen())
	{
		sf::Event ev;

		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
				case sf::Event::MouseMoved:
					brush.updateOnMouseMove(
						sf::Vector2i(ev.mouseMove.x, ev.mouseMove.y),
						canvas,
						camera.getPosition(),
						camera.getZoom());
					break;
				case sf::Event::MouseButtonPressed:
					brush.updateOnMouseButtonPress(
						ev.mouseButton.button,
						canvas);
					break;
				case sf::Event::MouseButtonReleased:
					brush.updateOnMouseButtonRelease(ev.mouseButton.button);
					break;
				case sf::Event::EventType::Closed:
					window.close();
					break;
			}
		}

		const float cameraZoom = camera.getZoom();

		sf::Transform transform;
		transform.scale(sf::Vector2f(cameraZoom, cameraZoom));
		transform.translate(-camera.getPosition());

		window.clear(clearColor);
		window.draw(canvas, transform);
		window.display();
	}

	return 0;
}