#include "Driver.h"
#include "ModifierKeys.h"
#include "Config.h"
#include <NFD/nfd.h>
#include <SFML/Graphics/RenderTarget.hpp>

Driver::Driver() :
	sf::Drawable(),
	threadPool_(),
	canvas_(sf::Vector2u(32, 32)),
	brush_(),
	palette_(),
	camera_(),
	mousePosition_(0, 0),
	modifierKeys_(ModifierKeys::None)
{
	threadPool_.start();

	palette_.load("./palettes/pico-8.txt");

	this->resetCamera_();
}

Driver::~Driver()
{
	threadPool_.stop();
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
		camera_.getZoom(),
		modifierKeys_);

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
		canvas_,
		modifierKeys_);

	palette_.updateOnMousePress(
		button,
		mousePosition_.x, mousePosition_.y,
		modifierKeys_);
}

void Driver::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	camera_.updateOnMouseButtonRelease(button);

	brush_.updateOnMouseButtonRelease(button);
}

void Driver::updateOnKeyPress(sf::Keyboard::Key key)
{
	switch (key)
	{
		case sf::Keyboard::Key::C:
			this->resetCamera_();
			break;
		case sf::Keyboard::Key::G:
			canvas_.toggleGrid();
			break;
		case sf::Keyboard::Key::S:
			threadPool_.push(std::bind(&Driver::startSaveDialog_, this));
			break;
		case sf::Keyboard::Key::L:
			threadPool_.push(std::bind(&Driver::startOpenDialog_, this));
			break;
		case sf::Keyboard::Key::LControl:
		case sf::Keyboard::Key::RControl:
			modifierKeys_ |= ModifierKeys::Control;
			break;
		case sf::Keyboard::Key::LAlt:
		case sf::Keyboard::Key::RAlt:
			modifierKeys_ |= ModifierKeys::Alt;
			break;
		case sf::Keyboard::Key::LShift:
		case sf::Keyboard::Key::RShift:
			modifierKeys_ |= ModifierKeys::Shift;
			break;
	}
}

void Driver::updateOnKeyRelease(sf::Keyboard::Key key)
{
	switch (key)
	{
		case sf::Keyboard::Key::LControl:
		case sf::Keyboard::Key::RControl:
			modifierKeys_ &= ~ModifierKeys::Control;
			break;
		case sf::Keyboard::Key::LAlt:
		case sf::Keyboard::Key::RAlt:
			modifierKeys_ &= ~ModifierKeys::Alt;
			break;
		case sf::Keyboard::Key::LShift:
		case sf::Keyboard::Key::RShift:
			modifierKeys_ &= ~ModifierKeys::Shift;
			break;
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

void Driver::startOpenDialog_()
{
	NFD_Init();

	nfdchar_t *outPath = NULL;
	nfdfilteritem_t filterItem[1] = { {"PNG file (.png)", "png"} };

	nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);

	if (result == NFD_OKAY)
	{
		canvas_.load(outPath);
		this->resetCamera_();

		NFD_FreePath(outPath);
	}

	NFD_Quit();
}

void Driver::startSaveDialog_()
{
	NFD_Init();

	nfdchar_t *savePath;
	nfdfilteritem_t filterItem[1] = { {"PNG file (.png)", "png"} };

	nfdresult_t result = NFD_SaveDialog(&savePath, filterItem, 1, NULL, "Untitled.png");

	if (result == NFD_OKAY)
	{
		canvas_.save(savePath);

		NFD_FreePath(savePath);
	}

	NFD_Quit();
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