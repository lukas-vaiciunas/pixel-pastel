#include "Canvas.h"
#include <SFML/Graphics/RenderTarget.hpp>

Canvas::Canvas(const sf::Vector2u &size) :
	sf::Drawable(),
	vertices_(sf::PrimitiveType::Triangles),
	outlineVertices_(sf::PrimitiveType::Triangles),
	size_(size)
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

void Canvas::init_()
{
	const sf::Color fillColor = sf::Color(255, 255, 255);

	for (unsigned int y = 0; y < size_.y; ++y)
	{
		for (unsigned int x = 0; x < size_.x; ++x)
		{
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						static_cast<float>(x),
						static_cast<float>(y)),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						static_cast<float>(x + 1),
						static_cast<float>(y)),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						static_cast<float>(x),
						static_cast<float>(y + 1)),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						static_cast<float>(x),
						static_cast<float>(y + 1)),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						static_cast<float>(x + 1),
						static_cast<float>(y)),
					fillColor));
			
			vertices_.append(
				sf::Vertex(
					sf::Vector2f(
						static_cast<float>(x + 1),
						static_cast<float>(y + 1)),
					fillColor));
		}
	}

	this->initOutline_();
}

void Canvas::initOutline_()
{
	const sf::Color color(0, 0, 0);
	const float thickness = 1.0f;
	
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
				size_.x + thickness,
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
				size_.x + thickness,
				-thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x + thickness,
				thickness),
			color));

	// Bottom

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				size_.y - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x + thickness,
				size_.y - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				size_.y + thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				-thickness,
				size_.y + thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x + thickness,
				size_.y - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x + thickness,
				size_.y + thickness),
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
				size_.y - thickness),
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
				size_.y - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				thickness,
				size_.y - thickness),
			color));

	// Right

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x - thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x - thickness,
				size_.y - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x + thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x + thickness,
				thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x - thickness,
				size_.y - thickness),
			color));

	outlineVertices_.append(
		sf::Vertex(
			sf::Vector2f(
				size_.x + thickness,
				size_.y - thickness),
			color));
}