#pragma once

#include "TextureId.h"
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Button :
	public sf::Drawable,
	public sf::Transformable
{
public:
	Button(const sf::Vector2f &size, TextureId iconTextureId);

	void updateOnMouseMove(int mouseX, int mouseY);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;

	void setCenterPosition(const sf::Vector2f &position);

	bool isHovered() const;
private:
	const sf::Texture &texture_;

	const sf::Vector2f size_;

	sf::VertexArray panelVertices_;
	sf::VertexArray outlineVertices_;
	sf::VertexArray iconVertices_;

	bool isHovered_;

	void init_();
	void initPanel_();
	void initOutline_();
	void initIcon_();
};