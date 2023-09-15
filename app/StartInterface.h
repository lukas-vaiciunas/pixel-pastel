#pragma once

#include "Panel.h"
#include "Button.h"
#include <SFML/Window/Mouse.hpp>

class StartInterface : public sf::Drawable
{
public:
	StartInterface(
		const sf::Vector2f &centerPosition,
		const sf::Vector2f &size);

	void updateOnMouseMove(int mouseX, int mouseY);
	void updateOnMouseButtonPress(sf::Mouse::Button button);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;
private:
	Panel panel_;

	Button createButton_;
	Button loadButton_;
};