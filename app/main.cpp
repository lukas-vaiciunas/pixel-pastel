#include "Canvas.h"
#include "Brush.h"
#include "Palette.h"
#include "Camera.h"
#include "EventQueue.h"
#include "Config.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	const sf::Color clearColor = sf::Color(205, 180, 219);

	sf::RenderWindow window(
		sf::VideoMode(
			Config::Window::width,
			Config::Window::height,
			Config::Window::bitsPerPixel),
		"Pixel Pastel",
		sf::Style::Titlebar | sf::Style::Close);

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	Canvas canvas(sf::Vector2u(32, 32));
	Brush brush;
	Palette palette;
	Camera camera;

	palette.load("./palettes/pico-8.txt");

	const sf::Vector2u &canvasSize = canvas.getSize();

	camera.setZoom(
		std::fminf(
			Config::Window::width / static_cast<float>(canvasSize.x),
			Config::Window::height / static_cast<float>(canvasSize.y)) * 0.75f);

	camera.setPosition(
		sf::Vector2f(
			(canvasSize.x - Config::Window::width / camera.getZoom()) * 0.5f,
			(canvasSize.y - Config::Window::height / camera.getZoom()) * 0.5f));

	sf::Vector2i lastMousePosition(0, 0);

	while (window.isOpen())
	{
		sf::Event ev;

		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
				case sf::Event::MouseMoved:
					camera.updateOnMouseMove(
						ev.mouseMove.x - lastMousePosition.x,
						ev.mouseMove.y - lastMousePosition.y);

					brush.updateOnMouseMove(
						ev.mouseMove.x, ev.mouseMove.y,
						canvas,
						camera.getPosition(),
						camera.getZoom());

					lastMousePosition.x = ev.mouseMove.x;
					lastMousePosition.y = ev.mouseMove.y;
					break;
				case sf::Event::MouseWheelScrolled:
					camera.updateOnMouseWheelScroll(
						ev.mouseWheelScroll.delta,
						ev.mouseWheelScroll.x, ev.mouseWheelScroll.y);
					break;
				case sf::Event::MouseButtonPressed:
					camera.updateOnMouseButtonPress(ev.mouseButton.button);

					brush.updateOnMouseButtonPress(
						ev.mouseButton.button,
						canvas);

					palette.updateOnMousePress(
						ev.mouseButton.button,
						ev.mouseButton.x, ev.mouseButton.y);
					break;
				case sf::Event::MouseButtonReleased:
					camera.updateOnMouseButtonRelease(ev.mouseButton.button);

					brush.updateOnMouseButtonRelease(ev.mouseButton.button);
					break;
				case sf::Event::EventType::Closed:
					window.close();
					break;
			}
		}

		EventQueue::getInstance().dispatch();

		const float cameraZoom = camera.getZoom();

		sf::Transform cameraTransform;
		cameraTransform.scale(sf::Vector2f(cameraZoom, cameraZoom));
		cameraTransform.translate(-camera.getPosition());

		sf::Transform interfaceTransform;
		interfaceTransform.scale(
			sf::Vector2f(
				Config::Interface::scale,
				Config::Interface::scale));

		window.clear(clearColor);
		window.draw(canvas, cameraTransform);
		window.draw(palette, interfaceTransform);
		window.display();
	}

	return 0;
}