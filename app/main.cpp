#include "Canvas.h"
#include "Brush.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	const sf::Color clearColor = sf::Color(255, 0, 255);

	sf::RenderWindow window(
		sf::VideoMode(1600, 900, 32),
		"Pixel Pastel",
		sf::Style::Titlebar | sf::Style::Close);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	const sf::Vector2f cameraPosition(-32.0f, -12.0f);
	const float cameraZoom = 16.0f;

	Canvas canvas(sf::Vector2u(32, 32));
	Brush brush;

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
						cameraPosition,
						cameraZoom);
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

		sf::Transform transform;
		transform.scale(sf::Vector2f(cameraZoom, cameraZoom));
		transform.translate(-cameraPosition);

		window.clear(clearColor);
		window.draw(canvas, transform);
		window.display();
	}

	return 0;
}