#pragma once
#include "sdk.h"

namespace modules
{
	void drawName(int playerIndex, Vector2& vScreen, Vector2& screenHead);
	void drawHealth(int playerIndex, Vector2& vScreen, Vector2& screenHead);
	void drawBoundingBox(int playerIndex, Vector2& vScreen, Vector2& screenHead);
	void drawEyeLines(int playerIndex, float matrix[16]);
	void ESP();
}