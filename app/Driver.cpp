#include "Driver.h"
#include "Config.h"
#include <SFML/Graphics/RenderTarget.hpp>

Driver::Driver() :
	sf::Drawable(),
	canvas_(sf::Vector2u(32, 32)),
	brush_(),
	palette_(),
	camera_(),
	mousePosition_(0, 0)
{
	this->resetCamera_();

	palette_.load("./palettes/pico-8.txt");
}

void Driver::updateOnMouseMove(int mouseX, int mouseY)
{
	camera_.updateOnMouseMove(
		mouseX - mousePosition_.x,
		mouseY - mousePosition_.y);

	brush_.updateOnMouseMove(
		mouseX, mouseY,
		canvas_,
		camera_.getPosition(),
		camera_.getZoom());

	mousePosition_.x = mouseX;
	mousePosition_.y = mouseY;
}

void Driver::updateOnMouseWheelScroll(float delta)
{
	camera_.updateOnMouseWheelScroll(
		delta,
		mousePosition_.x, mousePosition_.y);
}

void Driver::updateOnMouseButtonPress(sf::Mouse::Button button)
{
	camera_.updateOnMouseButtonPress(button);

	brush_.updateOnMouseButtonPress(
		button,
		canvas_);

	palette_.updateOnMousePress(
		button,
		mousePosition_.x, mousePosition_.y);
}

void Driver::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	camera_.updateOnMouseButtonRelease(button);

	brush_.updateOnMouseButtonRelease(button);
}

void Driver::updateOnKeyPress(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::C)
	{
		this->resetCamera_();
	}
	else if (key == sf::Keyboard::G)
	{
		canvas_.toggleGrid();
	}
}

void Driver::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	this->drawCameraTransform_(target, states);
	this->drawInterfaceTransform_(target, states);
}

void Driver::resetCamera_()
{
	const sf::Vector2u canvasPixelSize = canvas_.getSize() * canvas_.getCellSize();

	camera_.setZoom(
		std::fminf(
			Config::Window::width / static_cast<float>(canvasPixelSize.x),
			Config::Window::height / static_cast<float>(canvasPixelSize.y)) * 0.75f);

	camera_.setPosition(
		sf::Vector2f(
			(canvasPixelSize.x - Config::Window::width / camera_.getZoom()) * 0.5f,
			(canvasPixelSize.y - Config::Window::height / camera_.getZoom()) * 0.5f));
}

void Driver::drawCameraTransform_(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	const float cameraZoom = camera_.getZoom();

	states.transform.scale(sf::Vector2f(cameraZoom, cameraZoom));
	states.transform.translate(-camera_.getPosition());

	target.draw(canvas_, states);
}

void Driver::drawInterfaceTransform_(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	states.transform.scale(
		sf::Vector2f(
			Config::Interface::scale,
			Config::Interface::scale));

	target.draw(palette_, states);
}