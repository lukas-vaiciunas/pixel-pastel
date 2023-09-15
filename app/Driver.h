#pragma once

#include "Listener.h"
#include "DriverState.h"

class Driver :
	public DriverInterface,
	public Listener
{
public:
	Driver();

	~Driver();

	void onEvent(const Event &ev) override;

	void updateOnMouseMove(int mouseX, int mouseY) override;
	void updateOnMouseWheelScroll(float delta) override;
	void updateOnMouseButtonPress(sf::Mouse::Button button) override;
	void updateOnMouseButtonRelease(sf::Mouse::Button button) override;
	void updateOnKeyPress(sf::Keyboard::Key key) override;
	void updateOnKeyRelease(sf::Keyboard::Key key) override;

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;
private:
	DriverState *state_;
};