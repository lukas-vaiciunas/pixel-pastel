#pragma once

#include "DriverState.h"
#include "Canvas.h"
#include "Brush.h"
#include "Palette.h"
#include "Camera.h"

class EditDriverState : public DriverState
{
public:
	EditDriverState();
	EditDriverState(const std::string &filePath);

	void updateOnMouseMove(int mouseX, int mouseY) override;
	void updateOnMouseWheelScroll(float delta) override;
	void updateOnMouseButtonPress(sf::Mouse::Button button) override;
	void updateOnMouseButtonRelease(sf::Mouse::Button button) override;
	void updateOnKeyPress(sf::Keyboard::Key key) override;
	void updateOnKeyRelease(sf::Keyboard::Key key) override;

	void draw(
		sf::RenderTarget &target,
		sf::RenderStates states) const override;
private:
	Canvas canvas_;
	Brush brush_;
	Palette palette_;
	Camera camera_;

	uint8_t modifierKeys_;

	sf::Vector2i mousePosition_;

	void init_();

	void startOpenDialog_();
	void startSaveDialog_();

	void resetCamera_();

	void drawCameraTransform_(
		sf::RenderTarget &target,
		sf::RenderStates states) const;

	void drawInterfaceTransform_(
		sf::RenderTarget &target,
		sf::RenderStates states) const;
};