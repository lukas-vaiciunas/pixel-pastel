#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>

class Canvas : public sf::Drawable
{
public:
	Canvas(const sf::Vector2u &size);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;

	void setColor(
		const sf::Vector2u &position,
		const sf::Color &color);

	const sf::Vector2u &getSize() const;
	unsigned int getCellSize() const;
private:
	sf::VertexArray vertices_;
	sf::VertexArray transparencyVertices_;
	sf::VertexArray outlineVertices_;

	sf::Texture transparencyTexture_;

	sf::Vector2u size_;

	unsigned int cellSize_;

	void init_();
	void initOutline_();

	void addQuad(
		sf::VertexArray &vertices,
		const sf::Vector2f &minPosition,
		const sf::Vector2f &maxPosition,
		const sf::Color &color);
};