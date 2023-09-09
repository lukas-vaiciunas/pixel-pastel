#include "Palette.h"
#include "Event.h"
#include "EventQueue.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Config.h"
#include <fstream>
#include <sstream>

Palette::Palette() :
	sf::Drawable(),
	vertices_(sf::Triangles),
	numCols_(0),
	boxSize_(32)
{}

void Palette::updateOnMousePress(
	sf::Mouse::Button button,
	int mouseX, int mouseY)
{
	if (button != sf::Mouse::Button::Left)
	{
		return;
	}

	float invInterfaceScale = 1.0f / Config::Interface::scale;

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
		static_cast<unsigned int>(relativeMousePosition.x / boxSize_),
		static_cast<unsigned int>(relativeMousePosition.y / boxSize_));

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

		const sf::Vector2f position(
			static_cast<float>(col * boxSize_),
			static_cast<float>(row * boxSize_));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					position.x,
					position.y),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					position.x + boxSize_,
					position.y),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					position.x,
					position.y + boxSize_),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					position.x,
					position.y + boxSize_),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					position.x + boxSize_,
					position.y),
				color));

		vertices_.append(
			sf::Vertex(
				sf::Vector2f(
					position.x + boxSize_,
					position.y + boxSize_),
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