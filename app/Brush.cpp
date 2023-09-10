#include "Brush.h"
#include "Canvas.h"
#include "Event.h"

Brush::Brush() :
	Listener({ EventType::SetBrushColor }),
	color_(0, 0, 0),
	isInBounds_(false),
	isLeftMouseButtonPressed_(false),
	isRightMouseButtonPressed_(false),
	isControlPressed_(false)
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

	if (isLeftMouseButtonPressed_)
	{
		this->onLeftMouseButton_(canvas);
	}
	if (isRightMouseButtonPressed_)
	{
		this->onRightMouseButton_(canvas);
	}
}

void Brush::updateOnMouseButtonPress(
	sf::Mouse::Button button,
	Canvas &canvas)
{
	if (button == sf::Mouse::Button::Left)
	{
		isLeftMouseButtonPressed_ = true;

		this->onLeftMouseButton_(canvas);
	}
	else if (button == sf::Mouse::Button::Right)
	{
		isRightMouseButtonPressed_ = true;

		this->onRightMouseButton_(canvas);
	}
}

void Brush::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	if (button == sf::Mouse::Button::Left)
	{
		isLeftMouseButtonPressed_ = false;
	}
	else if (button == sf::Mouse::Button::Right)
	{
		isRightMouseButtonPressed_ = false;
	}
}

void Brush::updateOnKeyPress(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::LControl
		|| key == sf::Keyboard::Key::RControl)
	{
		isControlPressed_ = true;
	}
}

void Brush::updateOnKeyRelease(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::LControl
		|| key == sf::Keyboard::Key::RControl)
	{
		isControlPressed_ = false;
	}
}

void Brush::onLeftMouseButton_(Canvas &canvas)
{
	if (isControlPressed_)
	{
		this->pick_(canvas);
	}
	else
	{
		this->paint_(canvas);
	}
}

void Brush::onRightMouseButton_(Canvas &canvas)
{
	this->erase_(canvas);
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

void Brush::pick_(const Canvas &canvas)
{
	if (!isInBounds_)
	{
		return;
	}

	color_ = canvas.getColor(canvasPosition_);
}