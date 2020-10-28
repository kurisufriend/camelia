#pragma once

namespace hooks
{
	inline unsigned int getVirtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }
	bool setupHooks();
	bool destructHooks();
}