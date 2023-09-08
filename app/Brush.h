#pragma once

class Canvas;

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Color.hpp>

class Brush
{
public:
	Brush();

	void updateOnMouseMove(
		const sf::Vector2i &mousePosition,
		Canvas &canvas,
		const sf::Vector2f &cameraPosition,
		float cameraZoom);

	void updateOnMouseButtonPress(
		sf::Mouse::Button button,
		Canvas &canvas);

	void updateOnMouseButtonRelease(sf::Mouse::Button button);
private:
	sf::Color color_;

	sf::Vector2u canvasPosition_;

	bool isInBounds_;
	bool isPainting_;

	void paint_(Canvas &canvas);
};