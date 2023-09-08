#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

class Camera
{
public:
	Camera();

	void updateOnMouseMove(int deltaX, int deltaY);

	void updateOnMouseWheelScroll(
		float delta,
		int mouseX, int mouseY);

	void updateOnMouseButtonPress(sf::Mouse::Button button);
	void updateOnMouseButtonRelease(sf::Mouse::Button button);

	void setPosition(const sf::Vector2f &position);
	void setZoom(float zoom);

	const sf::Vector2f &getPosition() const;
	float getZoom() const;
private:
	sf::Vector2f position_;
	float zoom_;

	bool isMoving_;
};