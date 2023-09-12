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
		uint8_t modifierKeys);

	void load(const std::string &string);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;
private:
	sf::VertexArray vertices_;
	sf::VertexArray outlineVertices_;

	unsigned int numCols_;
	unsigned int boxSize_;
	float boxGap_;

	void addQuad_(
		sf::VertexArray &vertices,
		const sf::Vector2f &minPosition,
		const sf::Vector2f &maxPosition,
		const sf::Color &color);
};