#include "Brush.h"
#include "Canvas.h"
#include "Event.h"

Brush::Brush() :
	Listener({ EventType::SetBrushColor }),
	color_(0, 0, 0),
	isInBounds_(false),
	isPainting_(false),
	isErasing_(false)
{}

void Brush::onEvent(const Event &ev)
{
	color_ = static_cast<const EventSetBrushColor &>(ev).getColor();
}

void Brush::updateOnMouseMove(
	int mouseX, int mouseY,
	Canvas &canvas,
	const sf::Vector2f &cameraPosition,
	float cameraZoom)
{
	const sf::Vector2u &canvasSize = canvas.getSize();
	const unsigned int canvasCellSize = canvas.getCellSize();

	const sf::Vector2f mouseRelativePosition(
		mouseX / cameraZoom + cameraPosition.x,
		mouseY / cameraZoom + cameraPosition.y);

	isInBounds_ = mouseRelativePosition.x >= 0.0f
		&& mouseRelativePosition.x < canvasSize.x * canvasCellSize
		&& mouseRelativePosition.y >= 0.0f
		&& mouseRelativePosition.y < canvasSize.y * canvasCellSize;

	canvasPosition_.x = static_cast<unsigned int>(mouseRelativePosition.x / canvasCellSize);
	canvasPosition_.y = static_cast<unsigned int>(mouseRelativePosition.y / canvasCellSize);

	if (isPainting_)
	{
		this->paint_(canvas);
	}
	if (isErasing_)
	{
		this->erase_(canvas);
	}
}

void Brush::updateOnMouseButtonPress(
	sf::Mouse::Button button,
	Canvas &canvas)
{
	if (button == sf::Mouse::Button::Left)
	{
		isPainting_ = true;

		this->paint_(canvas);
	}
	else if (button == sf::Mouse::Button::Right)
	{
		isErasing_ = true;

		this->erase_(canvas);
	}
}

void Brush::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	if (button == sf::Mouse::Button::Left)
	{
		isPainting_ = false;
	}
	else if (button == sf::Mouse::Button::Right)
	{
		isErasing_ = false;
	}
}

void Brush::paint_(Canvas &canvas)
{
	if (!isInBounds_)
	{
		return;
	}

	canvas.setColor(canvasPosition_, color_);
}

void Brush::erase_(Canvas &canvas)
{
	if (!isInBounds_)
	{
		return;
	}

	canvas.erase(canvasPosition_);
}