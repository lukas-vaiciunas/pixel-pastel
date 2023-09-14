#pragma once

#include <SFML/Graphics/VertexArray.hpp>

namespace FreeFunctions
{
	void addQuad(
		sf::VertexArray &vertices,
		const sf::Vector2f &minPosition,
		const sf::Vector2f &maxPosition,
		const sf::Color &color);
}