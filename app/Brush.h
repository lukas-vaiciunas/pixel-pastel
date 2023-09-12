#pragma once

class Canvas;

#include "Listener.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
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
		float cameraZoom,
		uint8_t modifierKeys);

	void updateOnMouseButtonPress(
		sf::Mouse::Button button,
		Canvas &canvas,
		uint8_t modifierKeys);

	void updateOnMouseButtonRelease(sf::Mouse::Button button);
private:
	sf::Color primaryColor_;
	sf::Color secondaryColor_;

	sf::Vector2u canvasPosition_;

	bool isInBounds_;

	bool isLeftMouseButtonPressed_;
	bool isRightMouseButtonPressed_;

	void onLeftMouseButton_(Canvas &canvas, uint8_t modifierKeys);
	void onRightMouseButton_(Canvas &canvas);

	void paint_(Canvas &canvas, const sf::Color &color);
	void fill_(Canvas &canvas, const sf::Color &color);
	void erase_(Canvas &canvas);
	void pick_(const Canvas &canvas, sf::Color &color);
};