#include "StartInterface.h"
#include "Event.h"
#include "EventQueue.h"
#include "Config.h"
#include <SFML/Graphics/RenderTarget.hpp>

StartInterface::StartInterface(
	const sf::Vector2f &centerPosition,
	const sf::Vector2f &size)
	:
	sf::Drawable(),
	panel_(size),
	createButton_(
		sf::Vector2f(
			Config::Window::width * 0.2f,
			Config::Window::height * 0.2f) / Config::Interface::scale,
		101),
	loadButton_(
		sf::Vector2f(
			Config::Window::width * 0.2f,
			Config::Window::height * 0.2f) / Config::Interface::scale,
		102)
{
	panel_.setCenterPosition(centerPosition);

	const sf::Vector2f &panelPosition = panel_.getPosition();

	createButton_.setCenterPosition(
		sf::Vector2f(
			panelPosition.x + size.x * 0.25f,
			panelPosition.y + size.y * 0.5f));

	loadButton_.setCenterPosition(
		sf::Vector2f(
			panelPosition.x + size.x * 0.75f,
			panelPosition.y + size.y * 0.5f));
}

void StartInterface::updateOnMouseMove(int mouseX, int mouseY)
{
	createButton_.updateOnMouseMove(mouseX, mouseY);
	loadButton_.updateOnMouseMove(mouseX, mouseY);
}

void StartInterface::updateOnMouseButtonPress(sf::Mouse::Button button)
{
	if (button != sf::Mouse::Button::Left)
	{
		return;
	}

	if (createButton_.isHovered())
	{
		EventQueue::getInstance().send(
			new EventCreateImage());
	}
	else if (loadButton_.isHovered())
	{
		EventQueue::getInstance().send(
			new EventLoadImage());
	}
}

void StartInterface::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	target.draw(panel_, states);
	target.draw(createButton_, states);
	target.draw(loadButton_, states);
}