#include "Canvas.h"
#include <SFML/Graphics/RenderTarget.hpp>

Canvas::Canvas(const sf::Vector2u &size) :
	sf::Drawable(),
	vertices_(sf::PrimitiveType::Triangles),
	outlineVertices_(sf::PrimitiveType::Triangles),
	size_(size),
	cellSize_(8)
{
	this->init_();
}

void Canvas::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	target.draw(outlineVertices_, states);
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
	const sf::Color fillColor = sf::Color(255, 255, 255);

	for (unsigned int y = 0; y < size_.y; ++y)
	{
		for (unsigned int x = 0; x < size_.x; ++x)
		{
			const sf::Vector2f position(
				static_cast<float>(x * cellSize_),
				static_cast<float>(y * cellSize_));

			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize_,
						position.y),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y + cellSize_),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x,
						position.y + cellSize_),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize_,
						position.y),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						position.x + cellSize_,
						position.y + cellSize_),
					fillColor));
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

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				-thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				-thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				-thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				thickness),
			color));

	// Bottom

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				height - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				height - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				height + thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				height + thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				height - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				height + thickness),
			color));

	// Left

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				height - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				height - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				thickness,
				height - thickness),
			color));

	// Right

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width - thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width - thickness,
				height - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width - thickness,
				height - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				width + thickness,
				height - thickness),
			color));
}