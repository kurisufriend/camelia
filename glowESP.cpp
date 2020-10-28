#include "glowESP.h"
#include "includes.h"
#include "sdk.h"
#include "globals.h"
#include "offsets.h"
#include "settings.h"

// doing lame non-sdk implementation to test interface, will fix later

void modules::glowESP()
{
	if (!settings::bESP)
		return;
	if (!settings::bGlowESP)
		return;
	if (!g::pentLocalPlayer)
		return;
	CGlowObjectManager* glowManager = (CGlowObjectManager*)*(DWORD*)(g::dwClientModule + hazedumper::signatures::dwGlowObjectManager);
	if (!glowManager)
		return;
	for (int i = 0; i < 64; i++)
	{
		CBaseEntity* currentEntity = interfaces::pacClientEntityList->GetClientEntity(i);
		if (!currentEntity)
			continue;
		if (currentEntity->getTeamNum() == g::pentLocalPlayer->getTeamNum())
			continue;
		CGlowObjectManager::GlowObjectDefinition_t* currentGlowEntity = (CGlowObjectManager::GlowObjectDefinition_t*)((DWORD)glowManager + (currentEntity->getGlowIndex() * 0x38));
		if (!currentGlowEntity)
			continue;
		currentGlowEntity->m_vGlowColor = Vector(1.f, 1.f, 1.f);
		currentGlowEntity->m_flGlowAlpha = 1.f;
		currentGlowEntity->m_bFullBloomRender = false;
		//currentGlowEntity->m_flBloomAmount = 1.f;
		currentGlowEntity->m_bRenderWhenOccluded = true;
		//currentGlowEntity->m_bRenderWhenUnoccluded = false;
	}
}