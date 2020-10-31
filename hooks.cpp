#include "hooks.h"
#include "includes.h"
#include "globals.h"
#include "offsets.h"
#include "sdk.h"
#include "minhook/MinHook.h"
#include "drawing.h"
#include "utils.h"
#include "modules.h"
#include "menu.h"
#include "settings.h"


typedef HRESULT(__stdcall* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
tEndScene oEndScene = 0;

typedef bool(__stdcall* tCreateMove)(float, CUserCmd*);
tCreateMove oCreateMove = 0;

typedef LRESULT(__stdcall* tWndProc)(HWND window, UINT msg, WPARAM wparm, LPARAM lparm);
tWndProc oWndProc = 0;

typedef void(__thiscall* tFrameStageNotify)(void*, clientFrameStage);
tFrameStageNotify oFrameStageNotify = 0;

typedef bool(__thiscall* tSvCheats)(void*);
tSvCheats oSvCheats = 0;

typedef void(__fastcall* tDrawModelExecute)(void*, int, void* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld);
tDrawModelExecute oDrawModelExecute = 0;

typedef void(__thiscall* tPaintTraverse)(IPanel*, unsigned int, bool, bool);
tPaintTraverse oPaintTraverse;

void __stdcall hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
	if (!g::pDevice)
		g::pDevice = o_pDevice;
	if (!g::font)
		D3DXCreateFont(g::pDevice, 17, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_MODERN, "Courier New", &g::font);
	modules::ESP();
	menu::render();
	oEndScene(g::pDevice);
}

bool __stdcall hkCreateMove(float frametime, CUserCmd* cmd)
{
	oCreateMove(frametime, cmd);
	if (!cmd || !cmd->commandnumber)
		return false;

	g::pentLocalPlayer = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(interfaces::pacEngineClient->GetLocalPlayer());

	//was in designer base
	DWORD* framePointer;
	__asm mov framePointer, ebp; // so apparently ebp always contains a pointer to the stack frame that always works
	//see: https://stackoverflow.com/questions/579262/what-is-the-purpose-of-the-ebp-frame-pointer-register
	bool& sendPacket = *reinterpret_cast<bool*>(*framePointer - 0x1C); // and we can get offset it to grab stuff off the stack. pretty cool.

	Vector oldViewangles = cmd->viewangles;
	float oldForwardmove = cmd->forwardmove;
	float oldSidemove = cmd->sidemove;

	//do stuff

	modules::autohop(cmd);
	modules::triggerbot(cmd);
	modules::antiAim(cmd);
	modules::aimbot(cmd);
	modules::fakeLag(cmd, sendPacket);

	utils::correctMovement(oldViewangles, cmd, oldForwardmove, oldSidemove);

	modules::slowWalk(cmd);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);
	cmd->viewangles.Normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;
	if (sendPacket)
		g::realAngles = cmd->viewangles; // at least semi-accurately represent server side stuffs
	return false;
}

LRESULT __stdcall hkWndProc(HWND window, UINT msg, WPARAM wparm, LPARAM lparm)
{
	if (msg == WM_MOUSEMOVE)
	{
		g::mousePosition.x = LOWORD(lparm);
		g::mousePosition.y = HIWORD(lparm);
	}
	//if (g::menuOpen)
	//	return TRUE;
	return CallWindowProcA(oWndProc, window, msg, wparm, lparm);
}

void __stdcall hkFrameStageNotify(clientFrameStage frameStage)
{
	if (frameStage == FRAME_RENDER_END)
	{
		modules::glowESP(); // glow on renderend should eliminate flicker in theory
	}
	else if (frameStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		modules::knifechanger();
	}
	else if (frameStage == FRAME_RENDER_START)
	{
		if (g::pentLocalPlayer && settings::bAngleFix && interfaces::pacInput->cameraInThirdPerson)
		{
			g::pentLocalPlayer->setLocalVAngles(g::realAngles); //not an animfix so you can spot the change in anim from firstperson
		}// also yknow not an animfix doesnt accurately represent server side angles its just meant for seeing your aa
		modules::knifechanger();
		modules::thirdperson();
		modules::nightmode();
		modules::forceCrosshair();
		modules::recoilCrosshair();
		modules::grenadePrediction();
	}
	oFrameStageNotify(interfaces::pacClient, frameStage);
}

//from gladiatorz, if checked from camthink thirdperson return true
//always returning true breaks stuff check what exactly it does later
bool __fastcall hkSvCheats(PVOID cvarPtr, int edx)
{
	static DWORD CAM_THINK = (DWORD)utils::findPattern("client.dll", "85 C0 75 30 38 86");
	if (!cvarPtr)
		return false;

	if (settings::bThirdPerson && (DWORD)_ReturnAddress() == CAM_THINK)
	{
		return true;
	}

	return oSvCheats(cvarPtr);
}

void __fastcall hkDrawModelExecute(void* _this, int edx, void* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, void* pCustomBoneToWorld)
{
	// do chams elsewhere later, we have to call the original function
	if (!g::pentLocalPlayer)
		return;
	if (!pInfo.pModel)
		return;
	const char* name = interfaces::pacModelInfoClient->GetModelName(pInfo.pModel);
	IMaterial* material = interfaces::pacMaterialSystem->FindMaterial("debug/debugambientcube");
	if (strstr(name, "models/player") && material && settings::bPlayerChams)
	{
		CBasePlayer* curEntity = (CBasePlayer*)interfaces::pacClientEntityList->GetClientEntity(pInfo.entity_index);
		if (!curEntity || curEntity->getDormant() || curEntity->getHealth() < 0 || curEntity->getTeamNum() == g::pentLocalPlayer->getTeamNum())
		{
			oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
			return;
		}
		material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, !settings::bESPOnVisible);
		material->ColorModulate(1.f, 1.f, 0.f);
		material->AlphaModulate(1.f);

		interfaces::pacModelRender->ForcedMaterialOverride(material);
		oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
		interfaces::pacModelRender->ForcedMaterialOverride(nullptr);
	}
	else if (strstr(name + 17, "arms") && material && settings::bArmChams)
	{
		material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
		material->ColorModulate(.56f, .56f, .81f);
		material->AlphaModulate((!settings::bRemoveArms));

		interfaces::pacModelRender->ForcedMaterialOverride(material);
		oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
		interfaces::pacModelRender->ForcedMaterialOverride(nullptr);
	}
	else
	{
		oDrawModelExecute(_this, edx, ctx, state, pInfo, pCustomBoneToWorld);
	}
}

void __stdcall hkPaintTraverse(unsigned int panel, bool forceRepaint, bool allowForce)
{
	const char* panelName = interfaces::pacPanel->getPanelName(panel);
	if (!strcmp(panelName, "MatSystemTopPanel"))
	{
	}
	else if (!strcmp(panelName, "FocusOverlayPanel"))
	{
		interfaces::pacPanel->setMouseInputEnabled(panel, g::menuOpen);
		interfaces::pacPanel->setKeyboardInputEnabled(panel, g::menuOpen);
	}
	else if (!strcmp(panelName, "HudZoom") && settings::bRemoveScopeOverlay)
	{
		return;
	}
	oPaintTraverse(interfaces::pacPanel, panel, forceRepaint, forceRepaint);
}

bool hooks::setupHooks()
{
	std::cout << "reached hooks" << std::endl;

	IDirect3DDevice9* d3d9Device = *reinterpret_cast<IDirect3DDevice9**>(((DWORD)GetModuleHandle("shaderapidx9.dll") + hazedumper::signatures::dwppDirect3DDevice9));
	void** vTable = *reinterpret_cast<void***>(d3d9Device);
	void* EndScene = vTable[42];

	void* CreateMove = reinterpret_cast<void*>(getVirtual(interfaces::pacClientMode, 24));

	void* FrameStageNotify = reinterpret_cast<void*>(getVirtual(interfaces::pacClient, 37));

	void* SvCheats = reinterpret_cast<void*>(getVirtual(interfaces::pacCvar->FindVar("sv_cheats"), 13));

	void* DrawModelExecute = reinterpret_cast<void*>(getVirtual(interfaces::pacModelRender, 21));

	void* PaintTraverse = reinterpret_cast<void*>(getVirtual(interfaces::pacPanel, 41));

	std::cout << "initializing da hooking" << std::endl;

	if (MH_Initialize() != MH_OK)
		return false;

	std::cout << "hooking initialized" << std::endl;
	std::cout << "reached hook EndScene" << std::endl;

	if (MH_CreateHook(EndScene, &hkEndScene, reinterpret_cast<void**>(&oEndScene)) != MH_OK)
		return false;

	std::cout << "EndScene hooked" << std::endl;
	std::cout << "reached hook CreateMove" << std::endl;

	if (MH_CreateHook(CreateMove, &hkCreateMove, reinterpret_cast<void**>(&oCreateMove)) != MH_OK)
		return false;

	std::cout << "CreateMove hooked" << std::endl;
	std::cout << "reached hook WndProc" << std::endl;

	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongA(FindWindowA("Valve001", nullptr), GWL_WNDPROC, reinterpret_cast<LONG>(hkWndProc))); // bruhmoment21

	std::cout << "WndProc hooked" << std::endl;
	std::cout << "reached hook FrameStageNotfy" << std::endl;

	if (MH_CreateHook(FrameStageNotify, &hkFrameStageNotify, reinterpret_cast<void**>(&oFrameStageNotify)) != MH_OK)
		return false;

	std::cout << "FrameStageNotfy hooked" << std::endl;
	std::cout << "reached hook SvCheats" << std::endl;

	if (MH_CreateHook(SvCheats, &hkSvCheats, reinterpret_cast<void**>(&oSvCheats)) != MH_OK)
		return false;

	std::cout << "SvCheats hooked" << std::endl;
	std::cout << "reached hook DrawModelExecute" << std::endl;

	if (MH_CreateHook(DrawModelExecute, &hkDrawModelExecute, reinterpret_cast<void**>(&oDrawModelExecute)) != MH_OK)
		return false;

	std::cout << "DrawModelExecute hooked" << std::endl;
	std::cout << "reached hook PaintTraverse" << std::endl;

	if (MH_CreateHook(PaintTraverse, &hkPaintTraverse, reinterpret_cast<void**>(&oPaintTraverse)) != MH_OK)
		return false;

	std::cout << "PaintTraverse hooked" << std::endl;
	std::cout << "reached enable hooks" << std::endl;

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		return false;

	std::cout << "hooks enabled" << std::endl;
	std::cout << "leaving hooks" << std::endl;

	return true;
}

bool hooks::destructHooks()
{
	SetWindowLongA(FindWindowA("Valve001", 0), GWL_WNDPROC, reinterpret_cast<LONG>(oWndProc));
	if (MH_Uninitialize() != MH_OK) // i mean it returns an MH_STATUS so why not check it
		return false;
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
		return false;
	return true;
}