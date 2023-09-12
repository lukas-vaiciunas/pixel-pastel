#pragma once

#include <cstdint>

enum ModifierKeys : uint8_t
{
	None = 0,
	Control = 1 << 0,
	Alt = 1 << 1,
	Shift = 1 << 2
};