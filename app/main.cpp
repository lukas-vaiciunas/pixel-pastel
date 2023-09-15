#include "Driver.h"
#include "EventQueue.h"
#include "Config.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	const sf::Color clearColor = sf::Color(207, 186, 240);

	sf::RenderWindow window(
		sf::VideoMode(
			Config::Window::width,
			Config::Window::height,
			Config::Window::bitsPerPixel),
		"Pixel Pastel",
		sf::Style::Titlebar | sf::Style::Close);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	Driver driver;

	while (window.isOpen())
	{
		sf::Event ev;

		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
				case sf::Event::MouseMoved:
					driver.updateOnMouseMove(ev.mouseMove.x, ev.mouseMove.y);
					break;
				case sf::Event::MouseWheelScrolled:
					driver.updateOnMouseWheelScroll(ev.mouseWheelScroll.delta);
					break;
				case sf::Event::MouseButtonPressed:
					driver.updateOnMouseButtonPress(ev.mouseButton.button);
					break;
				case sf::Event::MouseButtonReleased:
					driver.updateOnMouseButtonRelease(ev.mouseButton.button);
					break;
				case sf::Event::KeyPressed:
					driver.updateOnKeyPress(ev.key.code);
					break;
				case sf::Event::KeyReleased:
					driver.updateOnKeyRelease(ev.key.code);
					break;
				case sf::Event::EventType::Closed:
					window.close();
					break;
			}
		}

		EventQueue::getInstance().dispatch();

		window.clear(clearColor);
		window.draw(driver);
		window.display();
	}

	return 0;
}