#pragma once

class IPanel {
public:
	void setKeyboardInputEnabled(unsigned int panel_id, bool state) 
	{
		using original_fn = void(__thiscall*)(IPanel*, unsigned int, bool);
		return (*(original_fn**)this)[31](this, panel_id, state);
	}

	void setMouseInputEnabled(unsigned int panel_id, bool state)
	{
		using original_fn = void(__thiscall*)(IPanel*, unsigned int, bool);
		return (*(original_fn**)this)[32](this, panel_id, state);
	}

	const char* getPanelName(unsigned int id)
	{
		using original_fn = const char* (__thiscall*)(IPanel*, unsigned int);
		return (*(original_fn**)this)[36](this, id);
	}
};