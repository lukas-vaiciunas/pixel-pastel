#pragma once

#include "EventType.h"
#include <SFML/Graphics/Color.hpp>
#include <string>

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

class EventEditFromCreate : public Event
{
public:
	EventEditFromCreate() :
		Event(EventType::EditFromCreate)
	{}
};

class EventEditFromLoad : public Event
{
public:
	EventEditFromLoad(const std::string &filePath) :
		Event(EventType::EditFromLoad),
		filePath_(filePath)
	{}

	const std::string &getFilePath() const
	{
		return filePath_;
	}
private:
	std::string filePath_;
};

class EventCreateImage : public Event
{
public:
	EventCreateImage() :
		Event(EventType::CreateImage)
	{}
};

class EventLoadImage : public Event
{
public:
	EventLoadImage() :
		Event(EventType::LoadImage)
	{}
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