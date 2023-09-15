#pragma once

#include "Panel.h"
#include "TextureId.h"

class Button : public Panel
{
public:
	Button(
		const sf::Vector2f &size,
		const sf::Color &fillColor,
		TextureId iconTextureId);

	void updateOnMouseMove(int mouseX, int mouseY);

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;

	bool isHovered() const;
private:
	const sf::Texture &texture_;

	sf::VertexArray iconVertices_;

	bool isHovered_;

	void init_();
};