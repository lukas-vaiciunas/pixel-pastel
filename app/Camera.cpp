#include "Camera.h"

Camera::Camera() :
	position_(-32.0f, -16.0f),
	zoom_(16.0f),
	isMoving_(false)
{}

void Camera::updateOnMouseMove(int deltaX, int deltaY)
{
	if (!isMoving_)
	{
		return;
	}

	position_.x -= deltaX / zoom_;
	position_.y -= deltaY / zoom_;
}

void Camera::updateOnMouseWheelScroll(
	float delta,
	int mouseX, int mouseY)
{
	sf::Vector2f mousePosition(
		static_cast<float>(mouseX),
		static_cast<float>(mouseY));

	position_ += mousePosition / zoom_;

	if (delta < 0.0f)
	{
		zoom_ *= 0.5f;
	}
	else if (delta > 0.0f)
	{
		zoom_ *= 1.5f;
	}

	position_ -= mousePosition / zoom_;
}

void Camera::updateOnMouseButtonPress(sf::Mouse::Button button)
{
	if (button != sf::Mouse::Button::Middle)
	{
		return;
	}

	isMoving_ = true;
}

void Camera::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	if (button != sf::Mouse::Button::Middle)
	{
		return;
	}

	isMoving_ = false;
}

void Camera::setPosition(const sf::Vector2f &position)
{
	position_ = position;
}

void Camera::setZoom(float zoom)
{
	zoom_ = zoom;
}

const sf::Vector2f &Camera::getPosition() const
{
	return position_;
}

float Camera::getZoom() const
{
	return zoom_;
}