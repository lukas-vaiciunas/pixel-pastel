#include "Brush.h"
#include "Canvas.h"

Brush::Brush() :
	color_(0, 0, 0),
	isInBounds_(false),
	isPainting_(false)
{}

void Brush::updateOnMouseMove(
	const sf::Vector2i &mousePosition,
	Canvas &canvas,
	const sf::Vector2f &cameraPosition,
	float cameraZoom)
{
	const sf::Vector2u &canvasSize = canvas.getSize();

	const sf::Vector2f mouseRelativePosition(
		mousePosition.x / cameraZoom + cameraPosition.x,
		mousePosition.y / cameraZoom + cameraPosition.y);

	isInBounds_ = mouseRelativePosition.x >= 0.0f
		&& mouseRelativePosition.x < canvasSize.x
		&& mouseRelativePosition.y >= 0.0f
		&& mouseRelativePosition.y < canvasSize.y;

	canvasPosition_.x = static_cast<unsigned int>(mouseRelativePosition.x);
	canvasPosition_.y = static_cast<unsigned int>(mouseRelativePosition.y);

	this->paint_(canvas);
}

void Brush::updateOnMouseButtonPress(
	sf::Mouse::Button button,
	Canvas &canvas)
{
	if (button != sf::Mouse::Button::Left)
	{
		return;
	}

	isPainting_ = true;

	this->paint_(canvas);
}

void Brush::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	if (button != sf::Mouse::Button::Left)
	{
		return;
	}

	isPainting_ = false;
}

void Brush::paint_(Canvas &canvas)
{
	if (!isInBounds_ || !isPainting_)
	{
		return;
	}

	canvas.setColor(canvasPosition_, color_);
}