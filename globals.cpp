#include "globals.h"

DWORD g::dwClientModule = 0;
CBasePlayer* g::pentLocalPlayer = 0;
LPDIRECT3DDEVICE9 g::pDevice = 0;
LPD3DXFONT g::font;
bool g::menuOpen = false;
Vector2 g::mousePosition;
Vector g::realAngles;

IClientEntityList* interfaces::pacClientEntityList = 0;
ICvar* interfaces::pacCvar = 0;
IEngineClient* interfaces::pacEngineClient = 0;
IBaseClientDll* interfaces::pacClient = 0;
IClientMode* interfaces::pacClientMode = 0;
IEngineTrace* interfaces::pacEngineTrace = 0;
IInput* interfaces::pacInput = 0;
IVModelInfoClient* interfaces::pacModelInfoClient = 0;
IVModelRender* interfaces::pacModelRender = 0;
IMaterialSystem* interfaces::pacMaterialSystem = 0;
IPanel* interfaces::pacPanel = 0;
ISurface* interfaces::pacSurface = 0;
IGlobals* interfaces::pacGlobals = 0;
IFileSystem* interfaces::pacFileSystem = 0;