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

class EventSetBrushColor : public Event
{
public:
	EventSetBrushColor(const sf::Color &color) :
		Event(EventType::SetBrushColor),
		color_(color)
	{}

	const sf::Color &getColor() const
	{
		return color_;
	}
private:
	sf::Color color_;
};