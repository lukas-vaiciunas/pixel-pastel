#include "FreeFunctions.h"

namespace FreeFunctions
{
	void addQuad(
		sf::VertexArray &vertices,
		const sf::Vector2f &minPosition,
		const sf::Vector2f &maxPosition,
		const sf::Color &color)
	{
		vertices.append(
			sf::Vertex(
				minPosition,
				color));

		vertices.append(
			sf::Vertex(
				sf::Vector2f(
					maxPosition.x,
					minPosition.y),
				color));

		vertices.append(
			sf::Vertex(
				sf::Vector2f(
					minPosition.x,
					maxPosition.y),
				color));

		vertices.append(
			sf::Vertex(
				sf::Vector2f(
					minPosition.x,
					maxPosition.y),
				color));

		vertices.append(
			sf::Vertex(
				sf::Vector2f(
					maxPosition.x,
					minPosition.y),
				color));

		vertices.append(
			sf::Vertex(
				maxPosition,
				color));
	}
}