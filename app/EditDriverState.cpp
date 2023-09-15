#include "EditDriverState.h"
#include "ThreadPool.h"
#include "ModifierKeys.h"
#include "Config.h"
#include <NFD/nfd.h>
#include <SFML/Graphics/RenderTarget.hpp>

EditDriverState::EditDriverState() :
	DriverState(),
	canvas_(sf::Vector2u(32, 32)),
	brush_(),
	palette_(),
	camera_(),
	mousePosition_(0, 0),
	modifierKeys_(ModifierKeys::None)
{
	this->init_();
}

EditDriverState::EditDriverState(const std::string &filePath) :
	DriverState(),
	canvas_(filePath),
	brush_(),
	palette_(),
	camera_(),
	mousePosition_(0, 0),
	modifierKeys_(ModifierKeys::None)
{
	this->init_();
}

void EditDriverState::updateOnMouseMove(int mouseX, int mouseY)
{
	const int deltaX = mouseX - mousePosition_.x;
	const int deltaY = mouseY - mousePosition_.y;

	mousePosition_.x = mouseX;
	mousePosition_.y = mouseY;

	camera_.updateOnMouseMove(deltaX, deltaY);

	brush_.updateOnMouseMove(
		mouseX, mouseY,
		canvas_,
		camera_.getPosition(),
		camera_.getZoom(),
		modifierKeys_);
}

void EditDriverState::updateOnMouseWheelScroll(float delta)
{
	camera_.updateOnMouseWheelScroll(
		delta,
		mousePosition_.x, mousePosition_.y);
}

void EditDriverState::updateOnMouseButtonPress(sf::Mouse::Button button)
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

void EditDriverState::updateOnMouseButtonRelease(sf::Mouse::Button button)
{
	camera_.updateOnMouseButtonRelease(button);

	brush_.updateOnMouseButtonRelease(button);
}

void EditDriverState::updateOnKeyPress(sf::Keyboard::Key key)
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
			ThreadPool::getInstance().push(std::bind(&EditDriverState::startSaveDialog_, this));
			break;
		case sf::Keyboard::Key::L:
			ThreadPool::getInstance().push(std::bind(&EditDriverState::startOpenDialog_, this));
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

void EditDriverState::updateOnKeyRelease(sf::Keyboard::Key key)
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

void EditDriverState::draw(
	sf::RenderTarget &target,
	sf::RenderStates states) const 
{
	this->drawCameraTransform_(target, states);
	this->drawInterfaceTransform_(target, states);
}

void EditDriverState::init_()
{
	palette_.load("./palettes/pico-8.txt");

	this->resetCamera_();
}

void EditDriverState::startOpenDialog_()
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

void EditDriverState::startSaveDialog_()
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

void EditDriverState::resetCamera_()
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

void EditDriverState::drawCameraTransform_(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	const float cameraZoom = camera_.getZoom();

	states.transform.scale(sf::Vector2f(cameraZoom, cameraZoom));
	states.transform.translate(-camera_.getPosition());

	target.draw(canvas_, states);
}

void EditDriverState::drawInterfaceTransform_(
	sf::RenderTarget &target,
	sf::RenderStates states) const
{
	states.transform.scale(
		sf::Vector2f(
			Config::Interface::scale,
			Config::Interface::scale));

	target.draw(palette_, states);
}