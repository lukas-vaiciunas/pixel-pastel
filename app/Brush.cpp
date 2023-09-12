#include "Brush.h"
#include "Canvas.h"
#include "Event.h"
#include "ModifierKeys.h"

Brush::Brush() :
	Listener(
		{
			EventType::SetBrushPrimaryColor,
			EventType::SetBrushSecondaryColor
		}),
	primaryColor_(255, 255, 255, 0),
	secondaryColor_(255, 255, 255, 0),
	isInBounds_(false),
	isLeftMouseButtonPressed_(false),
	isRightMouseButtonPressed_(false)
{}

void Brush::onEvent(const Event &ev)
{
	const EventType eventType = ev.getType();

	if (eventType == EventType::SetBrushPrimaryColor)
	{
		primaryColor_ = static_cast<const EventSetBrushPrimaryColor &>(ev).getColor();
	}
	else if (eventType == EventType::SetBrushSecondaryColor)
	{
		secondaryColor_ = static_cast<const EventSetBrushSecondaryColor &>(ev).getColor();
	}
}

void Brush::updateOnMouseMove(
	int mouseX, int mouseY,
	Canvas &canvas,
	const sf::Vector2f &cameraPosition,
	float cameraZoom,
	uint8_t modifierKeys)
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
		this->onLeftMouseButton_(canvas, modifierKeys);
	}
	if (isRightMouseButtonPressed_)
	{
		this->onRightMouseButton_(canvas, modifierKeys);
	}
}

void Brush::updateOnMouseButtonPress(
	sf::Mouse::Button button,
	Canvas &canvas,
	uint8_t modifierKeys)
{
	if (button == sf::Mouse::Button::Left)
	{
		isLeftMouseButtonPressed_ = true;

		this->onLeftMouseButton_(canvas, modifierKeys);
	}
	else if (button == sf::Mouse::Button::Right)
	{
		isRightMouseButtonPressed_ = true;

		this->onRightMouseButton_(canvas, modifierKeys);
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

void Brush::onLeftMouseButton_(Canvas &canvas, uint8_t modifierKeys)
{
	if (!isInBounds_)
	{
		return;
	}

	sf::Color &color = (modifierKeys & ModifierKeys::Alt ? secondaryColor_ : primaryColor_);

	if (modifierKeys & ModifierKeys::Control)
	{
		this->pick_(canvas, color);
	}
	else if (modifierKeys & ModifierKeys::Shift)
	{
		this->fill_(canvas, color);
	}
	else
	{
		this->paint_(canvas, color);
	}
}

void Brush::onRightMouseButton_(Canvas &canvas, uint8_t modifierKeys)
{
	if (!isInBounds_)
	{
		return;
	}

	if (modifierKeys & ModifierKeys::Shift)
	{
		this->fill_(canvas, sf::Color(255, 255, 255, 0));
	}
	else
	{
		this->erase_(canvas);
	}
}

void Brush::paint_(Canvas &canvas, const sf::Color &color)
{
	canvas.setColor(canvasPosition_, color);
}

void Brush::fill_(Canvas &canvas, const sf::Color &color)
{
	canvas.fill(canvasPosition_, color);
}

void Brush::erase_(Canvas &canvas)
{
	canvas.erase(canvasPosition_);
}

void Brush::pick_(const Canvas &canvas, sf::Color &color)
{
	color = canvas.getColor(canvasPosition_);
}