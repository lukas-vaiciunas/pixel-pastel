#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Panel :
	public sf::Drawable,
	public sf::Transformable
{
public:
	Panel(const sf::Vector2f &size);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;

	void setCenterPosition(const sf::Vector2f &position);
private:
	sf::VertexArray vertices_;
	sf::VertexArray outlineVertices_;

	sf::Vector2f size_;

	void init_();
};