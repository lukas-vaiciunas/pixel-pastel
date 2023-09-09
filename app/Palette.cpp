#include "Palette.h"
#include "Event.h"
#include "EventQueue.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <fstream>
#include <sstream>

Palette::Palette() :
	sf::Drawable(),
	vertices_(sf::Triangles),
	numCols_(0)
{}

void Palette::updateOnMousePress(
	sf::Mouse::Button button,
	int mouseX, int mouseY,
	float interfaceScale)
{
	if (button != sf::Mouse::Button::Left)
	{
		return;
	}

	float invInterfaceScale = 1.0f / interfaceScale;

	const sf::Vector2f relativeMousePosition(
		mouseX * invInterfaceScale,
		mouseY * invInterfaceScale);

	const sf::FloatRect verticesBounds = vertices_.getBounds();

	if (relativeMousePosition.x < 0.0f
		|| relativeMousePosition.x >= verticesBounds.width
		|| relativeMousePosition.y < 0.0f
		|| relativeMousePosition.y >= verticesBounds.height)
	{
		return;
	}

	sf::Vector2u palettePosition(
		static_cast<unsigned int>(relativeMousePosition.x),
		static_cast<unsigned int>(relativeMousePosition.y));

	unsigned int width = numCols_ * 6;

	EventQueue::getInstance().send(
		new EventSetBrushColor(
			vertices_[palettePosition.x * 6 + palettePosition.y * width].color));
}

void Palette::load(const std::string &string)
{
	std::ifstream in(string);

	if (!in.is_open())
	{
		fprintf(stderr, "Failed to open \"%s\" in Palette::load\n", string.c_str());
		return;
	}

	vertices_.clear();

	std::string line;
	std::stringstream ss;

	std::getline(in, line);

	numCols_ = std::stoi(line);

	unsigned int col = 0;
	unsigned int row = 0;

	while (std::getline(in, line))
	{
		int r;
		int g;
		int b;

		ss.clear();
		ss.str(line);

		ss >> r >> g >> b;

		const sf::Color color(r, g, b);

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					static_cast<float>(col),
					static_cast<float>(row)),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					static_cast<float>(col + 1),
					static_cast<float>(row)),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					static_cast<float>(col),
					static_cast<float>(row + 1)),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					static_cast<float>(col),
					static_cast<float>(row + 1)),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					static_cast<float>(col + 1),
					static_cast<float>(row)),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					static_cast<float>(col + 1),
					static_cast<float>(row + 1)),
				color));

		if (++col >= numCols_)
		{
			col = 0;
			++row;
		}
	}

	in.close();
}

void Palette::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	target.draw(vertices_, states);
}