#pragma once
#include "sdk.h"

namespace utils
{
	CBasePlayer* getClosestTarget();
	Vector interp(Vector src, Vector dst, int factor);
}
namespace modules
{
	Vector RCS(const Vector& src);
	void knifebot(CUserCmd* cmd);
	void aimbot(CUserCmd* cmd);
}