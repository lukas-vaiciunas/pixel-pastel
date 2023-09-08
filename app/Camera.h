#pragma once

#include <SFML/System/Vector2.hpp>

class Camera
{
public:
	Camera();

	void setPosition(const sf::Vector2f &position);
	void setZoom(float zoom);

	const sf::Vector2f &getPosition() const;
	float getZoom() const;
private:
	sf::Vector2f position_;
	float zoom_;
};