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
	if (cmd->buttons & IN_ATTACK) //p // nades are thrown fully after releasting IN_ATTACK (?) so they end up getting thrown at our feet. assuming once they are no longer held they are properly thrown, don't aa/return if you're holding a nade? theres probably some good fix for this wiht fancy sdk stuff or timers
		return;
	static bool alt = false;
	Vector curAngles;
	interfaces::pacEngineClient->GetViewAngles(curAngles); // curAngles is used to base the yaw, so we can use this for at target aa later
	if (settings::bJitter)
		curAngles.y += alt ? settings::iJitterAmount : -settings::iJitterAmount;
	cmd->viewangles = Vector(settings::iAAX, (int)curAngles.y + settings::iAAY, 0.f);
	alt = !alt;
}

//should be called last because it does checks based on stuff modified earlier (namely IN_ATTACK)
void modules::fakeLag(CUserCmd* cmd, bool& sendPacket) // just choke on a cycle, later use visble do fancy things like spike on peek or something
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bFakelag)
		return;
	if (cmd->buttons & IN_ATTACK)
		return;
	static int choked = 0;
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