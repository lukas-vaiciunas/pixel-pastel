#include "Canvas.h"
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

	Canvas canvas(sf::Vector2u(32, 32));
	canvas.setColor(sf::Vector2u(1, 1), sf::Color(0, 0, 0));
	canvas.setColor(sf::Vector2u(2, 2), sf::Color(0, 0, 0));
	canvas.setColor(sf::Vector2u(3, 3), sf::Color(0, 0, 0));
	canvas.setColor(sf::Vector2u(4, 4), sf::Color(0, 0, 0));
	canvas.setColor(sf::Vector2u(5, 5), sf::Color(0, 0, 0));
	canvas.setColor(sf::Vector2u(6, 6), sf::Color(0, 0, 0));

	while (window.isOpen())
	{
		sf::Event ev;

		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::EventType::Closed)
			{
				window.close();
			}
		}

		sf::Transform transform;
		transform.scale(sf::Vector2f(16.0f, 16.0f));
		transform.translate(sf::Vector2f(32.0f, 12.0f));

		window.clear(clearColor);
		window.draw(canvas, transform);
		window.display();
	}

	return 0;
}