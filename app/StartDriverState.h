#pragma once

#include "DriverState.h"
#include "Listener.h"
#include "StartInterface.h"

class StartDriverState :
	public DriverState,
	public Listener
{
public:
	StartDriverState();

	void onEvent(const Event &ev) override;

	void updateOnMouseMove(int mouseX, int mouseY) override;
	void updateOnMouseButtonPress(sf::Mouse::Button button) override;

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;
private:
	StartInterface interface_;

	void startOpenDialog_();
};