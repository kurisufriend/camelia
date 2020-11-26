#pragma once
#include "includes.h"
#include "sdk.h"

namespace g // muh gamerfood nostalgia
{
	extern DWORD dwClientModule;
	extern CBasePlayer* pentLocalPlayer;
	extern LPDIRECT3DDEVICE9 pDevice;
	extern LPD3DXFONT font;
	extern LPD3DXFONT fontLarge;
	extern bool menuOpen;
	extern Vector2 mousePosition;
	extern Vector realAngles;
	extern std::vector<int> speedGraphShitFuck;
}
namespace interfaces
{
	extern IClientEntityList* pacClientEntityList;
	extern ICvar* pacCvar;
	extern IEngineClient* pacEngineClient;
	extern IBaseClientDll* pacClient;
	extern IClientMode* pacClientMode;
	extern IEngineTrace* pacEngineTrace;
	extern IInput* pacInput;
	extern IVModelInfoClient* pacModelInfoClient;
	extern IVModelRender* pacModelRender;
	extern IMaterialSystem* pacMaterialSystem;
	extern IPanel* pacPanel;
	extern ISurface* pacSurface;
	extern IGlobals* pacGlobals;
	extern IFileSystem* pacFileSystem;
	extern IVDebugOverlay* pacDebugOverlay;
	extern PhysicsSurfaceProps* pacPhysicsSurfaceProps;
}