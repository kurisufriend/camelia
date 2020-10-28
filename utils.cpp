#include "utils.h"
#include "globals.h"
#include "includes.h"
#include "hooks.h"

#define INRANGE(x,a,b)    (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

bool utils::setup()
{
	std::cout << "reached setup" << std::endl;
	g::dwClientModule = (DWORD)GetModuleHandle("client.dll");
	if (!g::dwClientModule)
		return false;
	std::cout << "got module client.dll" << std::endl;
	if (!utils::setupInterfaces())
	{
		std::cout << "failed in interfaces" << std::endl;
		return false;
	}
	if (!hooks::setupHooks())
	{
		std::cout << "failed in hooks" << std::endl;
		return false;
	}
	std::cout << "leaving setup" << std::endl;
	return true;
}

// rake, createinterface is exported by most modules
void* utils::getInterface(const char* dllname, const char* interfacename)
{
	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);
	tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");

	int returnCode = 0;
	void* returnInterface = CreateInterface(interfacename, &returnCode);

	return returnInterface;
}

bool utils::setupInterfaces() // doing this here but hooks elsewhere triggers my tism
{
	std::cout << "reached interfaces" << std::endl;
	if (interfaces::pacClient = (IBaseClientDll*)getInterface("client.dll", "VClient018"); !interfaces::pacClient) // pretty sure if (interfaces::whatever = desu) would eval to the same thing but thats gay // wowie zowie, if with initializers! thanks c++17!
		return false;
	std::cout << "got interface IBaseClientDll" << std::endl;
	
	if (interfaces::pacClientEntityList = (IClientEntityList*)getInterface("client.dll", "VClientEntityList003"); !interfaces::pacClientEntityList)
		return false;
	std::cout << "got interface IClientEntityList" << std::endl;
	
	if (interfaces::pacCvar = (ICvar*)getInterface("vstdlib.dll", "VEngineCvar007"); !interfaces::pacCvar)
		return false;
	std::cout << "got interface ICvar" << std::endl;
	
	if (interfaces::pacEngineClient = (IEngineClient*)getInterface("engine.dll", "VEngineClient014"); !interfaces::pacEngineClient)
		return false;
	std::cout << "got interface IEngineClient" << std::endl;
	
	if (interfaces::pacEngineTrace = (IEngineTrace*)getInterface("engine.dll", "EngineTraceClient004"); !interfaces::pacEngineTrace)
		return false;
	std::cout << "got interface IEngineTrace" << std::endl;
	
	if (interfaces::pacModelInfoClient = (IVModelInfoClient*)getInterface("engine.dll", "VModelInfoClient004"); !interfaces::pacModelInfoClient)
		return false;
	std::cout << "got interface IVModelInfoClient" << std::endl;
	
	if (interfaces::pacModelRender = (IVModelRender*)getInterface("engine.dll", "VEngineModel016"); !interfaces::pacModelRender)
		return false;
	std::cout << "got interface IVModelRender" << std::endl;
	
	if (interfaces::pacMaterialSystem = (IMaterialSystem*)getInterface("materialsystem.dll", "VMaterialSystem080"); !interfaces::pacMaterialSystem)
		return false;
	std::cout << "got interface IMaterialSystem" << std::endl;
	
	if (interfaces::pacPanel = (IPanel*)getInterface("vgui2.dll", "VGUI_Panel009"); !interfaces::pacPanel)
		return false;
	std::cout << "got interface IPanel" << std::endl;
	
	if (interfaces::pacSurface = (ISurface*)getInterface("vguimatsurface.dll", "VGUI_Surface031"); !interfaces::pacSurface)
		return false;
	std::cout << "got interface ISurface" << std::endl;
	
	if (interfaces::pacClientMode = **(IClientMode***)((*reinterpret_cast<uintptr_t**>(interfaces::pacClient))[10] + 5); !interfaces::pacClientMode)
		return false;
	std::cout << "got interface IClientMode" << std::endl;
	
	if (interfaces::pacGlobals = **(IGlobals***)((*reinterpret_cast<uintptr_t**>(interfaces::pacClient))[11] + 10); !interfaces::pacGlobals)
		return false;
	std::cout << "got interface IGlobals" << std::endl;
	
	if (interfaces::pacInput = *(IInput**)(findPattern("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1); !interfaces::pacInput)
		return false;
	std::cout << "got interface IInput" << std::endl;
	std::cout << "leaving interfaces" << std::endl;
	return true;
}

// A5hack, takes ida style pattern. probably should change so it takes base addy instead of calcing modules every call
DWORD utils::findPattern(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2; //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

//aimtux
void utils::correctMovement(Vector oldAngles, CUserCmd* cmd, float oldForwardmove, float oldSidemove) {
	float delta_view;
	float f1;
	float f2;

	if (oldAngles.y < 0.f)
		f1 = 360.0f + oldAngles.y;
	else
		f1 = oldAngles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * oldForwardmove + cos(DEG2RAD(delta_view + 90.f)) * oldSidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * oldForwardmove + sin(DEG2RAD(delta_view + 90.f)) * oldSidemove;
}

//rake (?)
bool utils::worldToScreen(Vector pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}


// designer
Vector utils::calcAngle(const Vector& a, const Vector& b) {
	Vector angles;
	Vector delta;
	delta.x = (a.x - b.x);
	delta.y = (a.y - b.y);
	delta.z = (a.z - b.z);

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);

	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;
}

bool utils::isVisible(CBasePlayer* from, CBasePlayer* to)
{
	Vector eyepos = from->getEyePosition();
	Vector targeteyepos = to->getBonePosition(8);

	CGameTrace trace;
	Ray_t ray;
	CTraceFilter tracefilter;
	tracefilter.pSkip = (void*)from; // don't hit start player

	ray.Init(eyepos, targeteyepos);

	interfaces::pacEngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &tracefilter, &trace);
	return (trace.hit_entity == to || trace.fraction == 1.f); // if hit entity or did not hit surface, might fail with world entities?? check later
}

//https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/mathlib/mathlib_base.cpp#L901-L914
void utils::angleVectors(const Vector angles, Vector* forward)
{
	float sp, sy, cp, cy;
	sp = sin(DEG2RAD(angles.x));
	sy = sin(DEG2RAD(angles.y));
	cp = cos(DEG2RAD(angles.x));
	cy = cos(DEG2RAD(angles.y));
	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

bool utils::didHitNonWorldEntity(CBaseEntity* ent)
{
	return ent != NULL && ent != interfaces::pacClientEntityList->GetClientEntity(0);
}

float utils::getServerTime()
{
	return (g::pentLocalPlayer->getTickBase() * interfaces::pacGlobals->intervalPerTick);
}

bool utils::canShoot()
{
	CBaseWeapon* weapon = (CBaseWeapon*)interfaces::pacClientEntityList->GetClientEntityFromHandle(g::pentLocalPlayer->getCurrentWeapon());
	if (!weapon)
		return false;
	float nextShot = weapon->getNextPrimaryAttack() - utils::getServerTime();
	if (nextShot > 0)
		return false;
	return true;
}