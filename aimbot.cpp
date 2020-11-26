#include "aimbot.h"
#include "globals.h"
#include "settings.h"
#include "utils.h"
#include "includes.h"
#include "autowall.h"

CBasePlayer* getClosestTarget()
{
	float bestDistance = FLT_MAX; // i dunno how to get max float and too lazy to check //nvm for some reason LLONG exists but float needs to be shoetened???????
	CBasePlayer* bestPlayer = 0;
	for (int i = 0; i < 64; i++)
	{
		CBasePlayer* curPlayer = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(i);
		if (!curPlayer)
			continue;
		if (curPlayer->getTeamNum() != g::pentLocalPlayer->getTeamNum() && !curPlayer->getDormant() && (curPlayer->getHealth() > 0) && (utils::isVisible(g::pentLocalPlayer, curPlayer)))
		{
			Vector idealAngle = utils::calcAngle(g::pentLocalPlayer->getEyePosition(), curPlayer->getBonePosition(8));
			Vector curAngle;
			interfaces::pacEngineClient->GetViewAngles(curAngle);
			float curDistance = curAngle.DistTo(idealAngle);
			if (curDistance < bestDistance)
			{
				bestDistance = curDistance;
				bestPlayer = curPlayer;
			}
		}
	}
	return bestPlayer;
}

Vector RCS(const Vector& src) // put somewhere else maybe // also add factor for potential future legitness use + toggle for use
{
	return src - g::pentLocalPlayer->getAimPunchAngle() * 2.f;
}

Vector interp(Vector src, Vector dst, int factor)
{
	if (!factor)
		factor = 1;
	Vector delta = dst - src;
	delta.Normalize();
	delta /= (float)factor;
	return src + delta;
}

bool hitchance()
{
	CBaseWeapon* weapon = (CBaseWeapon*)interfaces::pacClientEntityList->GetClientEntityFromHandle(g::pentLocalPlayer->getCurrentWeapon());
	if (!weapon)
		return false;
	float inaccuracy = 1 / (weapon->getInaccuracy() + weapon->getSpread());
	if (inaccuracy < settings::iMinimumHitchance)
		return false;
	return true;
}

void modules::knifebot(CUserCmd* cmd)
{

}

void modules::aimbot(CUserCmd* cmd) //this misses legits for some reason, even with 100% accuracy. 
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bAimbot)
		return;
	if (!utils::canShoot())
		return;
	CBaseWeapon* weapon = (CBaseWeapon*)interfaces::pacClientEntityList->GetClientEntityFromHandle(g::pentLocalPlayer->getCurrentWeapon());
	if (weapon && weapon->getItemDefinitionIndex() == WEAPON_KNIFE || weapon->getItemDefinitionIndex() == WEAPON_KNIFE_T || weapon->getItemDefinitionIndex() == WEAPON_TASER) // we should be using a vfunc to check if its a knife or something
		return;
	CBasePlayer* target = getClosestTarget();
	if (!target)
		return;
	if (!utils::isVisible(g::pentLocalPlayer, target))
		return;
	if (!target->getDormant() && target->getHealth() > 0 && !target->getGunGameImmunity())
	{
		Vector angle = RCS(utils::calcAngle(g::pentLocalPlayer->getEyePosition(), target->getBonePosition(8)));
		//std::cout << modules::calculateAutowall(target, angle, weapon->getWeaponData()) << std::endl;
		Vector curAngle;
		interfaces::pacEngineClient->GetViewAngles(curAngle);
		if (!hitchance())
			return;
		if (settings::bAimbotSmoothing)
			angle = interp(curAngle, angle, settings::iSmoothingAmount);
		if (settings::bSilentAimbot)
			cmd->viewangles = angle;
		else
			interfaces::pacEngineClient->SetViewAngles(angle); // not using silent turns enemies into bullet dodging masters somehow?????????? ??? like seriously it misses people standing still?????????
		if (settings::bAutoShoot)
		{
			static bool shotLast = false;
			cmd->buttons = shotLast ? (cmd->buttons & ~IN_ATTACK) : (cmd->buttons | IN_ATTACK);
			shotLast = cmd->buttons & IN_ATTACK;
		}
	}
}