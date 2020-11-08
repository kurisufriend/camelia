#pragma once
#include "sdk.h"
#include "globals.h"

struct backtrackRecord
{
	int tickcount;
	float simtime;
	Vector headPos;
};

namespace modules
{
	void backtrack(CUserCmd* cmd);
}