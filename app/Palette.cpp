#include "Palette.h"
#include "FreeFunctions.h"
#include "Event.h"
#include "EventQueue.h"
#include "ModifierKeys.h"
#include "Config.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <fstream>
#include <sstream>

Palette::Palette() :
	sf::Drawable(),
	vertices_(sf::PrimitiveType::Triangles),
	outlineVertices_(sf::PrimitiveType::Triangles),
	numCols_(0),
	boxSize_(32),
	boxGap_(8.0f)
{}

void Palette::updateOnMousePress(
	sf::Mouse::Button button,
	int mouseX, int mouseY,
	uint8_t modifierKeys)
{
	if (button != sf::Mouse::Button::Left)
	{
		return;
	}

	const float invInterfaceScale = 1.0f / Config::Interface::scale;

	const sf::Vector2f relativeMousePosition(
		mouseX * invInterfaceScale,
		mouseY * invInterfaceScale);

	const sf::FloatRect verticesBounds = vertices_.getBounds();

	if (relativeMousePosition.x < boxGap_
		|| relativeMousePosition.x >= verticesBounds.width + boxGap_
		|| relativeMousePosition.y < boxGap_
		|| relativeMousePosition.y >= verticesBounds.height + boxGap_)
	{
		return;
	}

	const float boxOffset = boxSize_ + boxGap_;

	const sf::Vector2f paletteRatio(
		(relativeMousePosition.x - boxGap_) / boxOffset,
		(relativeMousePosition.y - boxGap_) / boxOffset);

	const float offsetMinRatio = boxSize_ / boxOffset;

	if (paletteRatio.x - floorf(paletteRatio.x) > offsetMinRatio
		|| paletteRatio.y - floorf(paletteRatio.y) > offsetMinRatio)
	{
		return;
	}

	const sf::Vector2u palettePosition(paletteRatio);

	const unsigned int width = numCols_ * 6;

	if (modifierKeys & ModifierKeys::Alt)
	{
		EventQueue::getInstance().send(
			new EventSetBrushSecondaryColor(
				vertices_[palettePosition.x * 6 + palettePosition.y * width].color));
	}
	else
	{
		EventQueue::getInstance().send(
			new EventSetBrushPrimaryColor(
				vertices_[palettePosition.x * 6 + palettePosition.y * width].color));
	}
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
	outlineVertices_.clear();

	std::string line;
	std::stringstream ss;

	std::getline(in, line);

	numCols_ = std::stoi(line);

	const float boxOffset = boxSize_ + boxGap_;
	const float outlineThickness = 2.0f;
	const sf::Color outlineColor(0, 0, 0);

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

		const sf::Vector2f minPosition(
			boxGap_ + col * boxOffset,
			boxGap_ + row * boxOffset);

		const sf::Vector2f maxPosition(
			minPosition.x + boxSize_,
			minPosition.y + boxSize_);

		FreeFunctions::addQuad(
			vertices_,
			minPosition,
			maxPosition,
			color);

		FreeFunctions::addOutline(
			outlineVertices_,
			minPosition,
			maxPosition,
			outlineColor,
			outlineThickness);

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
	target.draw(outlineVertices_, states);
	target.draw(vertices_, states);
}