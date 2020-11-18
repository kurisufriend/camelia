#pragma once
#include "sdk.h"
#include "globals.h"

struct backtrackRecord
{
	int tickcount;
	Vector headPos;
};

namespace modules
{
	void backtrack(CUserCmd* cmd);
}