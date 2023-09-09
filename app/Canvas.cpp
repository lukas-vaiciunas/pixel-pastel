#include "Canvas.h"
#include <SFML/Graphics/RenderTarget.hpp>

Canvas::Canvas(const sf::Vector2u &size) :
	sf::Drawable(),
	vertices_(sf::PrimitiveType::Triangles),
	transparencyVertices_(sf::PrimitiveType::Triangles),
	outlineVertices_(sf::PrimitiveType::Triangles),
	transparencyTexture_(),
	size_(size),
	cellSize_(8)
{
	if (!transparencyTexture_.loadFromFile("./assets/textures/transparency.png"))
	{
		fprintf(stderr, "Failed to open \"./assets/textures/transparency.png\" in Canvas::Canvas\n");
	}

	this->init_();
}

void Canvas::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	sf::RenderStates transparencyStates = states;
	transparencyStates.texture = &transparencyTexture_;

	target.draw(outlineVertices_, states);
	target.draw(transparencyVertices_, transparencyStates);
	target.draw(vertices_, states);
}

void Canvas::setColor(
	const sf::Vector2u &position,
	const sf::Color &color)
{
	const unsigned int width = size_.x * 6;
	const unsigned int base = position.x * 6 + position.y * width;

	if (vertices_[base].color == color)
	{
		return;
	}

	for (unsigned int i = base; i < base + 6; ++i)
	{
		vertices_[i].color = color;
	}
}

const sf::Vector2u &Canvas::getSize() const
{
	return size_;
}

unsigned int Canvas::getCellSize() const
{
	return cellSize_;
}

void Canvas::init_()
{
	const sf::Color fillColor = sf::Color(255, 255, 255, 0);

	for (unsigned int y = 0; y < size_.y; ++y)
	{
		for (unsigned int x = 0; x < size_.x; ++x)
		{
			float cellSize = static_cast<float>(cellSize_);

			const sf::Vector2f position(
				x * cellSize,
				y * cellSize);

			this->addQuad(
				vertices_,
				position,
				sf::Vector2f(position.x + cellSize, position.y + cellSize),
				fillColor);

			// Transparency

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y),
					sf::Vector2f(0.0f, 0.0f)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize,
						position.y),
					sf::Vector2f(cellSize, 0.0f)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y + cellSize),
					sf::Vector2f(0.0f, cellSize)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y + cellSize),
					sf::Vector2f(0.0f, cellSize)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize,
						position.y),
					sf::Vector2f(cellSize, 0.0f)));

			transparencyVertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize,
						position.y + cellSize),
					sf::Vector2f(cellSize, cellSize)));
		}
	}

	this->initOutline_();
}

void Canvas::initOutline_()
{
	const sf::Color color(0, 0, 0);
	const float thickness = 1.0f;
	unsigned int width = size_.x * cellSize_;
	unsigned int height = size_.y * cellSize_;
	
	// Top

	this->addQuad(
		outlineVertices_,
		sf::Vector2f(-thickness, -thickness),
		sf::Vector2f(width + thickness, thickness),
		color);

	// Bottom

	this->addQuad(
		outlineVertices_,
		sf::Vector2f(-thickness, height - thickness),
		sf::Vector2f(width + thickness, height + thickness),
		color);

	// Left

	this->addQuad(
		outlineVertices_,
		sf::Vector2f(-thickness, thickness),
		sf::Vector2f(thickness, height - thickness),
		color);

	// Right

	this->addQuad(
		outlineVertices_,
		sf::Vector2f(width - thickness, thickness),
		sf::Vector2f(width + thickness, height - thickness),
		color);
}

void Canvas::addQuad(
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