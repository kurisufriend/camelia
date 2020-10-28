#pragma once
#include "includes.h"

struct vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

namespace drawing
{
	void drawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
	void drawFilledRect2(int x, int y, int w, int h, D3DCOLOR color);
	void drawLine(float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color);
	void drawText(LPD3DXFONT font, unsigned int x, unsigned int y, D3DCOLOR fontColor, LPCSTR Message);
	int getTextWidth(const char* szText, LPD3DXFONT pFont);
}