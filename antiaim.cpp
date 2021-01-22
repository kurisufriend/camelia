#include "antiaim.h"
#include "settings.h"
#include "globals.h"
#include "sdk.h"
#include "utils.h"

void modules::antiAim(CUserCmd* cmd)
{
	if (!settings::bAntiAim)
		return;
	if (!g::pentLocalPlayer)
		return;
	if (g::pentLocalPlayer->getMoveType() == MOVETYPE_LADDER || g::pentLocalPlayer->getMoveType() == MOVETYPE_NOCLIP) // i can think of ways to fix movement on a ladder, but for now this works
		return;
	if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2 || cmd->buttons & IN_USE) //p // nades are thrown fully after releasting IN_ATTACK (?) so they end up getting thrown at our feet. assuming once they are no longer held they are properly thrown, don't aa/return if you're holding a nade? theres probably some good fix for this wiht fancy sdk stuff or timers
		return; // also resets when scoping in lmfao, maybe only return for atk2 when holding a knife?
	static bool alt = false;
	Vector curAngles;
	interfaces::pacEngineClient->GetViewAngles(curAngles); // curAngles is used to base the yaw, so we can use this for at target aa later
	if (settings::bJitter)
		curAngles.y += alt ? settings::iJitterAmount : -settings::iJitterAmount;
	cmd->viewangles = Vector(settings::iAAX, (int)curAngles.y + settings::iAAY, 0.f);
	alt = !alt;
}

void modules::fakeLag(CUserCmd* cmd, bool& sendPacket) // just choke on a cycle, later use visble do fancy things like spike on peek or something
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bFakelag)
		return;
	if (cmd->buttons & IN_ATTACK)
		return;
	static int choked = 0; // too poor to use clientstate
	if (choked < settings::iFakelagAmount)
	{
		sendPacket = false;
		choked++;
	}
	else
	{
		sendPacket = true;
		choked = 0;
	}
}