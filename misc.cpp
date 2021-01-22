#include "misc.h"
#include "sdk.h"
#include "includes.h"
#include "globals.h"
#include "offsets.h"
#include "settings.h"
#include "drawing.h"
#include "aimbot.h"
void modules::autohop(CUserCmd* cmd)
{
	if (!settings::bAutohop)
		return;
	if (!g::pentLocalPlayer)
		return;
	if (g::pentLocalPlayer->getMoveType() == MOVETYPE_LADDER) // don't hop if we're on a ladder. i've seen sources use MOVETYPE_NOCLIP or something but i'm not sure how that would interfere with anything
		return;
	if (!(g::pentLocalPlayer->getFlags() & FL_ONGROUND)) 
	{
		cmd->buttons &= ~IN_JUMP; // if i understand correctly the tilde op inverts IN_JUMP so we set the bits if we are not jumping and unset/release the button
	}
}
void modules::thirdperson()
{
	if (!settings::bThirdPerson)
		return;
	if (!g::pentLocalPlayer)
		return;

	if (GetAsyncKeyState(VK_MBUTTON) & 0x1)
		interfaces::pacInput->cameraInThirdPerson = !interfaces::pacInput->cameraInThirdPerson;
	interfaces::pacInput->cameraOffset.z = 100; // doesn't do anything in firstperson even if we change
}
void modules::nightmode()
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bNightmode)
		return;
	static ConVar* matForceTonemapScale = interfaces::pacCvar->FindVar("mat_force_tonemap_scale");
	static bool init = false;
	if (!init)
	{
		matForceTonemapScale->callbacks.size = 0;
		matForceTonemapScale->flags &= FCVAR_CHEAT;
	}
	matForceTonemapScale->setValue(settings::bNightmode ? ((settings::iNightmodeLevel / 10.f) + 0.f) : 0.f); // i dont know why its added to 0.f? probably remove otr something later
}
void modules::forceCrosshair()
{
	static ConVar* weaponDebugSpreadShow = interfaces::pacCvar->FindVar("weapon_debug_spread_show");
	static bool init = false;
	if (!init)
	{
		weaponDebugSpreadShow->callbacks.size = 0;
		weaponDebugSpreadShow->flags &= FCVAR_CHEAT;
	}
	weaponDebugSpreadShow->setValue(settings::bForceCrosshair ? 3 : 0);
}
void modules::recoilCrosshair()
{
	static ConVar* clCrosshairRecoil = interfaces::pacCvar->FindVar("cl_crosshair_recoil");
	static bool init = false;
	if (!init)
	{
		clCrosshairRecoil->callbacks.size = 0;
		clCrosshairRecoil->flags &= FCVAR_CHEAT;
	}
	clCrosshairRecoil->setValue(settings::bRecoilCrosshair);
}
void modules::grenadePrediction()
{
	static ConVar* clGrenadePreview = interfaces::pacCvar->FindVar("cl_grenadepreview");
	static bool init = false;
	if (!init)
	{
		clGrenadePreview->callbacks.size = 0;
		clGrenadePreview->flags &= FCVAR_REPLICATED;
		clGrenadePreview->flags &= FCVAR_CHEAT;
	}
	clGrenadePreview->setValue(settings::bGrenadePrediction);
}
void modules::slowWalk(CUserCmd* cmd)
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bSlowWalk)
		return;
	if (!GetAsyncKeyState(VK_SHIFT))
		return;
	cmd->forwardmove = std::clamp(cmd->forwardmove, (float)-settings::iSlowWalkAmount, (float)settings::iSlowWalkAmount);
	cmd->sidemove = std::clamp(cmd->sidemove, (float)-settings::iSlowWalkAmount, (float)settings::iSlowWalkAmount);
}
void modules::clantagChanger(CUserCmd* cmd)
{
	if (!g::pentLocalPlayer)
		return;
	if ((cmd->tickcount % 20) != 0)
		return;
	if (!settings::bClantagChanger)
	{
		//utils::setClanTag("");
		return;
	}
	utils::setClanTag(clantags.at(settings::iClantag).c_str());
}
void modules::autostrafer(CUserCmd* cmd)
{
	if (!settings::bAutostrafer)
		return;
	if (!GetAsyncKeyState(VK_SPACE))
		return;
	if (cmd->mousedx < 0)
		cmd->sidemove = -450.0f;
	else if (cmd->mousedx > 0)
		cmd->sidemove = 450.0f;
}
void modules::updateSpeedGraph() // called in cm
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bSpeedGraph)
		return;
	int vel = lrint(g::pentLocalPlayer->getVelocity().Length2D());
	g::speedGraphShitFuck.push_back(lrint(vel / 3));
	if (g::speedGraphShitFuck.size() > 200)
		g::speedGraphShitFuck.erase(g::speedGraphShitFuck.begin());
}
void modules::drawSpeedGraph() //endscene
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bSpeedGraph)
		return;
	int vel = lrint(g::pentLocalPlayer->getVelocity().Length2D());
	const char* velString = std::to_string(vel).c_str(); // charp because we only use it in text drawing and size// poor man's string conversion lmfao
	int velStringSize = drawing::getTextWidth(velString, g::fontLarge);
	int x = GetSystemMetrics(SM_CXSCREEN) / 2;
	int y = GetSystemMetrics(SM_CYSCREEN) - 50;
	drawing::drawText(g::fontLarge, x - (velStringSize / 2), y, D3DCOLOR_ARGB(255, 255, 255, 255), std::to_string(vel).c_str());
	//Vector2 oldPoint = Vector2((x + 0) - (g::speedGraphShitFuck.size() / 2), y - (g::speedGraphShitFuck.at(0)));
	for (int i = 0; i < g::speedGraphShitFuck.size(); i++)
	{
		drawing::drawPixel((x + i) - (g::speedGraphShitFuck.size() / 2), y - (g::speedGraphShitFuck.at(i)), D3DCOLOR_ARGB(255, 255, 255, 255));
		// lines lag the shit out of everything
		//Vector2 oldPoint = Vector2((x + 0) - (g::speedGraphShitFuck.size() / 2), y - (g::speedGraphShitFuck.at(0)));
		//Vector2 curPoint = Vector2((x + i) - (g::speedGraphShitFuck.size() / 2), y - (g::speedGraphShitFuck.at(i)));
		//drawing::drawLine(oldPoint.x, oldPoint.y, curPoint.x, curPoint.y, 1, true, D3DCOLOR_ARGB(255, 255, 255, 255));
		//oldPoint = curPoint;
	}
}
void modules::followbot(CUserCmd* cmd)
{
	if (!settings::bFollowbot)
		return;
	CBaseEntity* daRealPlayaNoCap = utils::getClosestTarget();
	if (!daRealPlayaNoCap) // make sure hes da real one no cap
		return;
	bool checkLole = utils::isVisible(g::pentLocalPlayer, (CBasePlayer*)daRealPlayaNoCap); // can we see dat nigga
	Vector angle = RCS(utils::calcAngle(g::pentLocalPlayer->getEyePosition(), daRealPlayaNoCap->getOrigin()));
	Vector curAngle;
	interfaces::pacEngineClient->GetViewAngles(curAngle);
	angle = utils::interp(curAngle, angle, 4);
	Vector legoNicecopter = Vector(curAngle.x, angle.y, curAngle.z);
	interfaces::pacEngineClient->SetViewAngles(legoNicecopter);
	cmd->forwardmove = 450;
}