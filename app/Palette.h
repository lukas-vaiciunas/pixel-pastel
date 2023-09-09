#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>

class Palette : public sf::Drawable
{
public:
	Palette();

	void updateOnMousePress(
		sf::Mouse::Button button,
		int mouseX, int mouseY,
		float interfaceScale);

	void load(const std::string &string);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;
private:
	sf::VertexArray vertices_;

	unsigned int numCols_;
};