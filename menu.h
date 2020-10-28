#pragma once
#include "drawing.h"
#include "includes.h"

namespace menu
{
	bool isMouseInRect(int x1, int y1, int x2, int y2);
	void drawButton(std::string label, int x1, int& y1, void(*callback)(), D3DCOLOR color);
	void drawToggle(std::string label, int x1, int& y1, bool& tracker, D3DCOLOR color);
	void drawTab(std::string label, int& x1, int y1, int& tracker, int tab);
	void drawSlider(std::string label, int x1, int& y1, int& tracker, int min, int max, D3DCOLOR color);
	void drawSeparator(int& y);
	void render();
}