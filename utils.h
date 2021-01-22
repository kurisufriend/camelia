#pragma once
#include "includes.h"
#include "sdk.h"
namespace utils
{
	bool setup();
	void* getInterface(const char* dllname, const char* interfacename);
	bool setupInterfaces();
	bool setupNetvars();
	DWORD findPattern(std::string module, std::string pattern);
	void correctMovement(Vector oldAngles, CUserCmd* cmd, float old_forwardmove, float old_sidemove);
	bool worldToScreen(Vector pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight);
	Vector calcAngle(const Vector& a, const Vector& b);
	bool isVisible(CBasePlayer* from, CBasePlayer* to);
	void traceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr);
    void angleVectors(const Vector angles, Vector* forward);
	bool didHitNonWorldEntity(CBaseEntity* ent);
	float getServerTime();
	bool canShoot();
	void setClanTag(const char* tag);
}