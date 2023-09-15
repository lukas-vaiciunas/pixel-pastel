#include "StartDriverState.h"
#include "Event.h"
#include "EventQueue.h"
#include "ThreadPool.h"
#include "Config.h"
#include <NFD/nfd.h>
#include <SFML/Graphics/RenderTarget.hpp>

StartDriverState::StartDriverState() :
	DriverState(),
	Listener(
		{
			EventType::CreateImage,
			EventType::LoadImage
		}),
	interface_(
		sf::Vector2f(
			Config::Window::width * 0.5f / Config::Interface::scale,
			Config::Window::height * 0.5f / Config::Interface::scale),
		sf::Vector2f(
			Config::Window::width * 0.5f / Config::Interface::scale,
			Config::Window::height * 0.5f / Config::Interface::scale))
{}

void StartDriverState::onEvent(const Event &ev)
{
	if (ev.getType() == EventType::CreateImage)
	{
		EventQueue::getInstance().send(
			new EventEditFromCreate());
	}
	else if (ev.getType() == EventType::LoadImage)
	{
		ThreadPool::getInstance().push(
			std::bind(&StartDriverState::startOpenDialog_, this));
	}
}

void StartDriverState::updateOnMouseMove(int mouseX, int mouseY)
{
	interface_.updateOnMouseMove(mouseX, mouseY);
}

void StartDriverState::updateOnMouseButtonPress(sf::Mouse::Button button)
{
	interface_.updateOnMouseButtonPress(button);
}

void StartDriverState::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	target.draw(interface_, states);
}

void StartDriverState::startOpenDialog_()
{
	NFD_Init();

	nfdchar_t *outPath = NULL;
	nfdfilteritem_t filterItem[1] = { {"PNG file (.png)", "png"} };

	nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);

	if (result == NFD_OKAY)
	{
		EventQueue::getInstance().send(
			new EventEditFromLoad(
				outPath));

		NFD_FreePath(outPath);
	}

	NFD_Quit();
}