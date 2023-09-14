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

	void addOutline(
		sf::VertexArray &vertices,
		const sf::Vector2f &minPosition,
		const sf::Vector2f &maxPosition,
		const sf::Color &color,
		float thickness)
	{
		addQuad(
			vertices,
			sf::Vector2f(minPosition.x - thickness, minPosition.y - thickness),
			sf::Vector2f(maxPosition.x + thickness, minPosition.y),
			color);

		addQuad(
			vertices,
			sf::Vector2f(minPosition.x - thickness, maxPosition.y),
			sf::Vector2f(maxPosition.x + thickness, maxPosition.y + thickness),
			color);

		addQuad(
			vertices,
			sf::Vector2f(minPosition.x - thickness, minPosition.y),
			sf::Vector2f(minPosition.x, maxPosition.y),
			color);

		addQuad(
			vertices,
			sf::Vector2f(maxPosition.x, minPosition.y),
			sf::Vector2f(maxPosition.x + thickness, maxPosition.y),
			color);
	}
}