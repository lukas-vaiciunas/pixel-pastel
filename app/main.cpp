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

		window.clear(clearColor);
		window.display();
	}

	return 0;
}