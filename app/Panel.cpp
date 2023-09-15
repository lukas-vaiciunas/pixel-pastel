#include "Panel.h"
#include "FreeFunctions.h"
#include <SFML/Graphics/RenderTarget.hpp>

Panel::Panel(const sf::Vector2f &size) :
	sf::Drawable(),
	sf::Transformable(),
	vertices_(sf::PrimitiveType::Triangles),
	outlineVertices_(sf::PrimitiveType::Triangles),
	size_(size)
{
	this->init_();
}

void Panel::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(outlineVertices_, states);
	target.draw(vertices_, states);
}

void Panel::setCenterPosition(const sf::Vector2f &position)
{
	this->setPosition(position - size_ * 0.5f);
}

void Panel::init_()
{
	vertices_.clear();
	outlineVertices_.clear();

	FreeFunctions::addQuad(
		vertices_,
		sf::Vector2f(0.0f, 0.0f),
		size_,
		sf::Color(236, 236, 236));

	FreeFunctions::addOutline(
		outlineVertices_,
		sf::Vector2f(0.0f, 0.0f),
		size_,
		sf::Color(0, 0, 0),
		2.0f);
}