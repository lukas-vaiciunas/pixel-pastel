#pragma once

#include <cstdint>

enum class EventType : uint8_t
{
	EditFromCreate,
	EditFromLoad,
	CreateImage,
	LoadImage,
	SetBrushPrimaryColor,
	SetBrushSecondaryColor
};