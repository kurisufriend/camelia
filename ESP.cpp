#include "ESP.h"
#include "includes.h"
#include "drawing.h"
#include "globals.h"
#include "utils.h"
#include "settings.h"

#define COLOR_RED D3DCOLOR_ARGB(255, 178, 34, 34)
#define COLOR_YELLOW D3DCOLOR_ARGB(255, 255, 165, 0)
#define COLOR_BBOX_YELLOW D3DCOLOR_ARGB(255, 255, 255, 0)
#define COLOR_GREEN D3DCOLOR_ARGB(255, 34, 139, 34)

void modules::drawName(int playerIndex, Vector2& vScreen, Vector2& screenHeadPos)
{
	player_info_t info;
	interfaces::pacEngineClient->GetPlayerInfo(playerIndex, &info);
	float height = std::abs(screenHeadPos.y - vScreen.y);
	drawing::drawText(g::font, (unsigned int)screenHeadPos.x - (height / 4), (unsigned int)vScreen.y, D3DCOLOR_ARGB(255, 255, 255, 255), (LPCSTR)info.name);
}
void modules::drawHealth(int playerIndex, Vector2 &vScreen, Vector2& screenHeadPos)
{
	CBasePlayer* player = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(playerIndex);
	float health = (float)player->getHealth();

	float height = std::abs(screenHeadPos.y - vScreen.y);

	drawing::drawFilledRect((int)screenHeadPos.x - (height / 4) - 2, (int)screenHeadPos.y - 8, (int)5, (int)height, D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DCOLOR color;
	if (health < 30)
		color = COLOR_RED;
	else if (health < 50)
		color = COLOR_YELLOW;
	else
		color = COLOR_GREEN;
	drawing::drawFilledRect((int)screenHeadPos.x - (height / 4) - 2, (int)screenHeadPos.y - 8, (int)5, (int)height * (float)(health / (float)100), color);
	Vector2 healthPos;
	healthPos.x = (int)screenHeadPos.x - (height / 4) - 40;
	healthPos.y = (int)screenHeadPos.y + (int)height * (float)(health / (float)100);
}
void modules::drawBoundingBox(int playerIndex, Vector2 &vScreen, Vector2& screenHeadPos)
{
	CBasePlayer* player = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(playerIndex);
	float health = (float)player->getHealth();

	float height = std::abs(screenHeadPos.y - vScreen.y);
	float width = height / 2;

	Vector2 topLeft;
	topLeft.x = screenHeadPos.x - (width / 2);
	topLeft.y = screenHeadPos.y - 8;
	Vector2 topRight;
	topRight.x = screenHeadPos.x + (width / 2);
	topRight.y = screenHeadPos.y - 8;
	Vector2 bottomLeft;
	bottomLeft.x = screenHeadPos.x - (width / 2);
	bottomLeft.y = vScreen.y;
	Vector2 bottomRight;
	bottomRight.x = screenHeadPos.x + (width / 2);
	bottomRight.y = vScreen.y;
	drawing::drawLine(topLeft.x, topLeft.y, topRight.x, topRight.y, 1, false, COLOR_BBOX_YELLOW);
	drawing::drawLine(bottomLeft.x, bottomLeft.y, bottomRight.x, bottomRight.y, 1, false, COLOR_BBOX_YELLOW);
	drawing::drawLine(topRight.x, topRight.y, bottomRight.x, bottomRight.y, 1, false, COLOR_BBOX_YELLOW);
	drawing::drawLine(topLeft.x, topLeft.y, bottomLeft.x, bottomLeft.y, 1, false, COLOR_BBOX_YELLOW);
}
void modules::drawEyeLines(int playerIndex, float matrix[16])
{
	CBasePlayer* player = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(playerIndex);
	Vector viewAngles = player->getViewAngles();

	Vector vecStart, vecEnd, vecForward;
	utils::angleVectors(viewAngles, &vecForward);

	vecStart = player->getBonePosition(8);
	vecForward *= 99999;
	vecEnd = vecStart + vecForward;

	CGameTrace trace;
	Ray_t ray;
	CTraceFilter tracefilter;
	tracefilter.pSkip = (void*)player;

	ray.Init(vecStart, vecEnd);

	interfaces::pacEngineTrace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &tracefilter, &trace);
	Vector2 lineStart;
	Vector2 lineEnd;
	utils::worldToScreen(vecStart, lineStart, matrix, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	utils::worldToScreen(trace.endpos, lineEnd, matrix, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	//drawing::drawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, 1, false, D3DCOLOR_ARGB(255, 255, 255, 255));
	interfaces::pacDebugOverlay->AddLineOverlay(vecStart, trace.endpos, 255, 255, 255, /*settings::bESPOnVisible*/false, -1);
}
//pastad from prev external software probably bad fix later // lolol it crashed fixed // basically all diff now
void modules::ESP()
{
	if (!settings::bESP)
		return;
	if (!g::pentLocalPlayer)
		return;
	float matrix[16];
	memcpy(&matrix, (PBYTE*)(g::dwClientModule + hazedumper::signatures::dwViewMatrix), sizeof(matrix));
	for (int i = 0; i < 64; i++)
	{
		CBasePlayer* curPlayer = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(i);
		if (!curPlayer)
			continue;
		Vector2 screenOrigin;
		Vector2 screenHeadPos;
		if (curPlayer->getHealth() > 0 && !curPlayer->getDormant() && (g::pentLocalPlayer->getTeamNum() != curPlayer->getTeamNum()))
		{
			if (settings::bESPOnVisible && !utils::isVisible(g::pentLocalPlayer, curPlayer))
				continue;
			if (!utils::worldToScreen(curPlayer->getOrigin(), screenOrigin, matrix, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)))
				continue;
			if (!utils::worldToScreen(curPlayer->getBonePosition(8), screenHeadPos, matrix, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)))
				continue;
			if (settings::bBoxESP)
				drawBoundingBox(i, screenOrigin, screenHeadPos);
			if (settings::bNameESP)
				drawName(i, screenOrigin, screenHeadPos);
			if (settings::bHealthESP)
				drawHealth(i, screenOrigin, screenHeadPos);
			if (settings::bBarrelESP)
				drawEyeLines(i, matrix);
		}
	}
}