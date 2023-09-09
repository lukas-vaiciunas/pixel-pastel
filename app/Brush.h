#pragma once

class Canvas;

#include "Listener.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Color.hpp>

class Brush : public Listener
{
public:
	Brush();

	void onEvent(const Event &ev) override;

	void updateOnMouseMove(
		int mouseX, int mouseY,
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
	bool isErasing_;

	void paint_(Canvas &canvas);
	void erase_(Canvas &canvas);
};