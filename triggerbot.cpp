#include "triggerbot.h"
#include "globals.h"
#include "utils.h"
#include "settings.h"

void modules::triggerbot(CUserCmd* cmd) // use traces later, i'm too brainlet for getting shot angle // owned thanks masterlooser for the anglevector function ;^)
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bTriggerbot)
		return;
	if (!GetAsyncKeyState(VK_XBUTTON2))
		return;
	Vector viewAngles = cmd->viewangles;
	viewAngles += g::pentLocalPlayer->getAimPunchAngle() * 2.f;

	Vector vecStart, vecEnd, vecForward;
	utils::angleVectors(viewAngles, &vecForward);

	vecStart = g::pentLocalPlayer->getEyePosition();
	vecForward *= 9999;
	vecEnd = vecStart + vecForward;

	CGameTrace trace;
	Ray_t ray;
	CTraceFilter tracefilter;
	tracefilter.pSkip = (void*)g::pentLocalPlayer;

	ray.Init(vecStart, vecEnd);

	interfaces::pacEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &tracefilter, &trace);

	CBaseEntity* hitEntity = (CBaseEntity*)trace.hit_entity;

	if (!hitEntity || hitEntity->getDormant() || (hitEntity->getTeamNum() == g::pentLocalPlayer->getTeamNum()))
		return;
	//if (!((trace.hitgroup == HITGROUP_HEAD) || (trace.hitgroup == HITBOX_CHEST) || (trace.hitgroup == HITGROUP_STOMACH) || (trace.hitgroup == HITGROUP_LEFTARM) || (trace.hitgroup == HITGROUP_RIGHTARM) || (trace.hitgroup == HITGROUP_LEFTLEG) || (trace.hitgroup == HITGROUP_RIGHTLEG)))
	//	return;
	
	static bool shotLast = false;
	cmd->buttons = shotLast ? (cmd->buttons & ~IN_ATTACK) : (cmd->buttons | IN_ATTACK);
	shotLast = cmd->buttons & IN_ATTACK;
}