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
		float cameraZoom);

	void updateOnMouseButtonPress(
		sf::Mouse::Button button,
		Canvas &canvas);

	void updateOnMouseButtonRelease(sf::Mouse::Button button);

	void updateOnKeyPress(sf::Keyboard::Key key);

	void updateOnKeyRelease(sf::Keyboard::Key key);
private:
	sf::Color color_;

	sf::Vector2u canvasPosition_;

	bool isInBounds_;

	bool isLeftMouseButtonPressed_;
	bool isRightMouseButtonPressed_;

	bool isControlPressed_;
	bool isShiftPressed_;

	void onLeftMouseButton_(Canvas &canvas);
	void onRightMouseButton_(Canvas &canvas);

	void paint_(Canvas &canvas);
	void fill_(Canvas &canvas);
	void erase_(Canvas &canvas);
	void pick_(const Canvas &canvas);
};