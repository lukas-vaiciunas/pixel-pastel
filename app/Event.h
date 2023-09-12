#pragma once

#include "EventType.h"
#include <SFML/Graphics/Color.hpp>

class Event
{
public:
	Event(EventType type) :
		type_(type)
	{}

	virtual ~Event() {}

	EventType getType() const
	{
		return type_;
	}
private:
	EventType type_;
};

class EventSetBrushPrimaryColor : public Event
{
public:
	EventSetBrushPrimaryColor(const sf::Color &color) :
		Event(EventType::SetBrushPrimaryColor),
		color_(color)
	{}

	const sf::Color &getColor() const
	{
		return color_;
	}
private:
	sf::Color color_;
};

class EventSetBrushSecondaryColor : public Event
{
public:
	EventSetBrushSecondaryColor(const sf::Color &color) :
		Event(EventType::SetBrushSecondaryColor),
		color_(color)
	{}

	const sf::Color &getColor() const
	{
		return color_;
	}
private:
	sf::Color color_;
};