#include "Camera.h"

Camera::Camera() :
	position_(-32.0f, -16.0f),
	zoom_(16.0f)
{}

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