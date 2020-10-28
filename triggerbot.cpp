#include "triggerbot.h"
#include "globals.h"
#include "utils.h"
#include "settings.h"

void modules::triggerbot(CUserCmd* cmd) // use traces later, i'm too brainlet for getting shot angle
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bTriggerbot)
		return;
	if (!GetAsyncKeyState(VK_XBUTTON2))
		return;
	int cID = g::pentLocalPlayer->getCrosshairID();
	CBasePlayer* cEntity = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(cID);
	if (cID && cEntity->getTeamNum() != g::pentLocalPlayer->getTeamNum() && cEntity->getHealth() > 0)
	{
		static bool shotLast = false;
		cmd->buttons = shotLast ? (cmd->buttons & ~IN_ATTACK) : (cmd->buttons | IN_ATTACK);
		shotLast = cmd->buttons & IN_ATTACK;
	}
}