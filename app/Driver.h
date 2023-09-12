#pragma once

#include "Canvas.h"
#include "Brush.h"
#include "Palette.h"
#include "Camera.h"

class Driver : public sf::Drawable
{
public:
	Driver();

	void updateOnMouseMove(int mouseX, int mouseY);
	void updateOnMouseWheelScroll(float delta);
	void updateOnMouseButtonPress(sf::Mouse::Button button);
	void updateOnMouseButtonRelease(sf::Mouse::Button button);
	void updateOnKeyPress(sf::Keyboard::Key key);
	void updateOnKeyRelease(sf::Keyboard::Key key);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;
private:
	Canvas canvas_;
	Brush brush_;
	Palette palette_;
	Camera camera_;

	sf::Vector2i mousePosition_;

	bool isAltPressed_;

	void resetCamera_();

	void drawCameraTransform_(
		sf::RenderTarget &target,
		sf::RenderStates states) const;

	void drawInterfaceTransform_(
		sf::RenderTarget &target,
		sf::RenderStates states) const;
};