#pragma once

#include "DriverInterface.h"

class DriverState : public DriverInterface
{
public:
	DriverState();

	virtual ~DriverState() {}

	virtual void updateOnMouseMove(int mouseX, int mouseY) override {}
	virtual void updateOnMouseWheelScroll(float delta) override {}
	virtual void updateOnMouseButtonPress(sf::Mouse::Button button) override {}
	virtual void updateOnMouseButtonRelease(sf::Mouse::Button button) override {}
	virtual void updateOnKeyPress(sf::Keyboard::Key key) override {}
	virtual void updateOnKeyRelease(sf::Keyboard::Key key) override {}

	virtual void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override {}
};