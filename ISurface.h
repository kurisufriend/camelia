#pragma once
class ISurface
{
public:
	void drawOutlinedCircle(int x, int y, int radius, int segments)
	{
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[103](this, x, y, radius, segments);
	}
};