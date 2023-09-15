#include "Button.h"
#include "TextureData.h"
#include "FreeFunctions.h"
#include "Config.h"
#include <SFML/Graphics/RenderTarget.hpp>

Button::Button(const sf::Vector2f &size, TextureId iconTextureId)
	:
	sf::Drawable(),
	sf::Transformable(),
	texture_(TextureData::getInstance().getTexture(iconTextureId)),
	size_(size),
	panelVertices_(sf::PrimitiveType::Triangles),
	outlineVertices_(sf::PrimitiveType::Triangles),
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
	states.transform *= this->getTransform();

	target.draw(outlineVertices_, states);
	target.draw(panelVertices_, states);

	states.texture = &texture_;

	target.draw(iconVertices_, states);
}

void Button::setCenterPosition(const sf::Vector2f &position)
{
	this->setPosition(position - size_ * 0.5f);
}

bool Button::isHovered() const
{
	return isHovered_;
}

void Button::init_()
{
	this->initPanel_();
	this->initOutline_();
	this->initIcon_();
}

void Button::initPanel_()
{
	panelVertices_.clear();

	FreeFunctions::addQuad(
		panelVertices_,
		sf::Vector2f(0.0f, 0.0f),
		size_,
		sf::Color(207, 186, 240));
}

void Button::initOutline_()
{
	outlineVertices_.clear();

	FreeFunctions::addOutline(
		outlineVertices_,
		sf::Vector2f(0.0f, 0.0f),
		size_,
		sf::Color(0, 0, 0),
		2.0f);
}

void Button::initIcon_()
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