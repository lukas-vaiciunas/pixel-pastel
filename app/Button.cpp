#include "Button.h"
#include "TextureData.h"
#include "FreeFunctions.h"
#include "Config.h"
#include <SFML/Graphics/RenderTarget.hpp>

Button::Button(
	const sf::Vector2f &size,
	const sf::Color &fillColor,
	TextureId iconTextureId)
	:
	Panel(size, fillColor),
	texture_(TextureData::getInstance().getTexture(iconTextureId)),
	iconVertices_(sf::PrimitiveType::Triangles),
	isHovered_(false)
{
	this->init_();
}

void Button::updateOnMouseMove(int mouseX, int mouseY)
{
	float invInterfaceScale = 1.0f / Config::Interface::scale;

	sf::Vector2f relativeMousePosition(
		mouseX * invInterfaceScale,
		mouseY * invInterfaceScale);

	const sf::Vector2f &position = getPosition();

	isHovered_ = relativeMousePosition.x > position.x
		&& relativeMousePosition.x < position.x + size_.x
		&& relativeMousePosition.y > position.y
		&& relativeMousePosition.y < position.y + size_.y;
}

void Button::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	Panel::draw(target, states);

	states.transform *= this->getTransform();
	states.texture = &texture_;

	target.draw(iconVertices_, states);
}

bool Button::isHovered() const
{
	return isHovered_;
}

void Button::init_()
{
	iconVertices_.clear();

	const sf::Vector2f textureSize(texture_.getSize());

	const sf::Vector2f halfSize = size_ * 0.5f;
	const sf::Vector2f halfTextureSize = textureSize * 0.5f * 8.0f;

	FreeFunctions::addTextureQuad(
		iconVertices_,
		halfSize - halfTextureSize,
		halfSize + halfTextureSize,
		textureSize);
}