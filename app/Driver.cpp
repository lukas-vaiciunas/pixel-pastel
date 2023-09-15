#include "Driver.h"
#include "StartDriverState.h"
#include "EditDriverState.h"
#include "Event.h"
#include "ThreadPool.h"
#include <SFML/Graphics/RenderTarget.hpp>

Driver::Driver() :
	DriverInterface(),
	Listener(
		{
			EventType::EditFromCreate,
			EventType::EditFromLoad
		}),
	state_(new StartDriverState())
{
	ThreadPool::getInstance().start();
}

Driver::~Driver()
{
	ThreadPool::getInstance().stop();

	delete state_;
}

void Driver::onEvent(const Event &ev)
{
	if (ev.getType() == EventType::EditFromCreate)
	{
		delete state_;

		state_ = new EditDriverState();
	}
	else if (ev.getType() == EventType::EditFromLoad)
	{
		delete state_;

		state_ = new EditDriverState(
			static_cast<const EventEditFromLoad &>(ev).getFilePath());
	}
}

void Driver::updateOnMouseMove(int mouseX, int mouseY)
{
	state_->updateOnMouseMove(mouseX, mouseY);
}

void Driver::updateOnMouseWheelScroll(float delta)
{
	state_->updateOnMouseWheelScroll(delta);
}

void Driver::updateOnMouseButtonPress(sf::Mouse::Button button)
{
	state_->updateOnMouseButtonPress(button);
}

void Driver::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	state_->updateOnMouseButtonRelease(button);
}

void Driver::updateOnKeyPress(sf::Keyboard::Key key)
{
	state_->updateOnKeyPress(key);
}

void Driver::updateOnKeyRelease(sf::Keyboard::Key key)
{
	state_->updateOnKeyRelease(key);
}

void Driver::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	target.draw(*state_, states);
}