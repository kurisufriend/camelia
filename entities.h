#pragma once
#include "includes.h"
#include "offsets.h"
#include "Vector.h"
#include "IEngineTrace.h"

// CBaseEntity::m_fFlags
// PLAYER SPECIFIC FLAGS FIRST BECAUSE WE USE ONLY A FEW BITS OF NETWORK PRECISION
// This top block is for singleplayer games only....no HL2:DM (which defines HL2_DLL)
#define FL_ONGROUND (1<<0) // At rest / on the ground
#define FL_DUCKING (1<<1) // Player flag -- Player is fully crouched
#define FL_WATERJUMP (1<<2) // player jumping out of water
#define FL_ONTRAIN (1<<3) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN (1<<4) // Indicates the entity is standing in rain
#define FL_FROZEN (1<<5) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS (1<<6) // Player can't move, but keeps key inputs for controlling another entity
#define FL_CLIENT (1<<7) // Is a player
#define FL_FAKECLIENT (1<<8) // Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define FL_INWATER (1<<9) // In water

enum MoveTypes : uint8_t
{
    MOVETYPE_NONE = 0,
    // never moves
    MOVETYPE_ISOMETRIC,
    // For players -- in TF2 commander view, etc.
    MOVETYPE_WALK,
    // player only - moving on the ground
    MOVETYPE_STEP,
    // gravity, special edge handling -- monsters use this
    MOVETYPE_FLY,
    // No gravity, but still collides with stuff
    MOVETYPE_FLYGRAVITY,
    // flies through the air + is affected by gravity
    MOVETYPE_VPHYSICS,
    // uses VPHYSICS for simulation
    MOVETYPE_PUSH,
    // no clip to world, push and crush
    MOVETYPE_NOCLIP,
    // No gravity, no collisions, still do velocity/avelocity
    MOVETYPE_LADDER,
    // Used by players only when going onto a ladder
    MOVETYPE_OBSERVER,
    // Observer movement, depends on player's observer mode
    MOVETYPE_CUSTOM,
    // Allows the entity to describe its own physics
    MOVETYPE_LAST = MOVETYPE_CUSTOM,
    // should always be defined as the last item in the list
    MOVETYPE_MAX_BITS = 4
};

class CBaseEntity
{
public:
    void* animating()
    {
        return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
    }
    int draw_model(int flags, uint8_t alpha)
    {
        using original_fn = int(__thiscall*)(void*, int, uint8_t);
        return (*(original_fn**)animating())[9](animating(), flags, alpha);
    }
    void setModelIndexVFunc(int value)
    {
        using fn = void(__thiscall*)(CBaseEntity*, int);
        return (*(fn**)this)[75](this, value);
    }
    int getFlags()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_fFlags);
    }
    unsigned long getOwnerEntity()
    {
        return *(unsigned long*)((DWORD)this + hazedumper::netvars::m_hOwnerEntity);
    }
    float getSimulationTime()
    {
        return *(float*)((DWORD)this + hazedumper::netvars::m_flSimulationTime);
    }
    Vector getOrigin()
    {
        return *(Vector*)((DWORD)this + hazedumper::netvars::m_vecOrigin);
    }
    Vector getViewOffset()
    {
        return *(Vector*)((DWORD)this + hazedumper::netvars::m_vecViewOffset);
    }
    int getTeamNum()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iTeamNum);
    }
    bool getSpotted()
    {
        return *(bool*)((DWORD)this + hazedumper::netvars::m_bSpotted);
    }
    int getGlowIndex()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iGlowIndex);
    }
    bool getDormant()
    {
        return *(int*)((DWORD)this + hazedumper::signatures::m_bDormant);
    }
    DWORD getViewmodel()
    {
        return *(DWORD*)((DWORD)this + hazedumper::netvars::m_hViewModel);
    }
    int getModelIndex()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_nModelIndex);
    }
    void setModelIndex(int value)
    {
        *(int*)((DWORD)this + hazedumper::netvars::m_nModelIndex) = value;
    }
};
static std::vector<std::string> knifechangerOptions =
{
    "bayonet",
    "classic",
    "flip",
    "gut",
    "karambit",
    "m9 bayonet",
    "huntsman",
    "falchion",
    "bowie",
    "butterfly",
    "shadow daggers",
    "paracord",
    "survival",
    "ursus",
    "navaja",
    "nomad",
    "stiletto",
    "talon",
    "skeleton"
};
static std::vector<int> knifeDefinitionIndices =
{
    503,
    505,
    506,
    507,
    508,
    509,
    512,
    514,
    515,
    516,
    519,
    520,
    522,
    523,
};
enum itemDefinition : int
{
    WEAPON_NONE = 0,
    WEAPON_DEAGLE,
    WEAPON_ELITE,
    WEAPON_FIVESEVEN,
    WEAPON_GLOCK,
    WEAPON_AK47 = 7,
    WEAPON_AUG,
    WEAPON_AWP,
    WEAPON_FAMAS,
    WEAPON_G3SG1,
    WEAPON_GALILAR = 13,
    WEAPON_M249,
    WEAPON_M4A1 = 16,
    WEAPON_MAC10,
    WEAPON_P90 = 19,
    WEAPON_MP5SD = 23,
    WEAPON_UMP45,
    WEAPON_XM1014,
    WEAPON_BIZON,
    WEAPON_MAG7,
    WEAPON_NEGEV,
    WEAPON_SAWEDOFF,
    WEAPON_TEC9,
    WEAPON_TASER,
    WEAPON_HKP2000,
    WEAPON_MP7,
    WEAPON_MP9,
    WEAPON_NOVA,
    WEAPON_P250,
    WEAPON_SHIELD,
    WEAPON_SCAR20,
    WEAPON_SG556,
    WEAPON_SSG08,
    WEAPON_KNIFEGG,
    WEAPON_KNIFE,
    WEAPON_FLASHBANG,
    WEAPON_HEGRENADE,
    WEAPON_SMOKEGRENADE,
    WEAPON_MOLOTOV,
    WEAPON_DECOY,
    WEAPON_INCGRENADE,
    WEAPON_C4,
    WEAPON_HEALTHSHOT = 57,
    WEAPON_KNIFE_T = 59,
    WEAPON_M4A1_SILENCER,
    WEAPON_USP_SILENCER,
    WEAPON_CZ75A = 63,
    WEAPON_REVOLVER,
    WEAPON_TAGRENADE = 68,
    WEAPON_FISTS,
    WEAPON_BREACHCHARGE,
    WEAPON_TABLET = 72,
    WEAPON_MELEE = 74,
    WEAPON_AXE,
    WEAPON_HAMMER,
    WEAPON_SPANNER = 78,
    WEAPON_KNIFE_GHOST = 80,
    WEAPON_FIREBOMB,
    WEAPON_DIVERSION,
    WEAPON_FRAG_GRENADE,
    WEAPON_SNOWBALL,
    WEAPON_BUMPMINE,
    WEAPON_BAYONET = 500,
    WEAPON_KNIFE_CSS = 503,
    WEAPON_KNIFE_FLIP = 505,
    WEAPON_KNIFE_GUT,
    WEAPON_KNIFE_KARAMBIT,
    WEAPON_KNIFE_M9_BAYONET,
    WEAPON_KNIFE_TACTICAL,
    WEAPON_KNIFE_FALCHION = 512,
    WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
    WEAPON_KNIFE_BUTTERFLY,
    WEAPON_KNIFE_PUSH,
    WEAPON_KNIFE_URSUS = 519,
    WEAPON_KNIFE_GYPSY_JACKKNIFE,
    WEAPON_KNIFE_STILETTO = 522,
    WEAPON_KNIFE_WIDOWMAKER,
    GLOVE_STUDDED_BLOODHOUND = 5027,
    GLOVE_T_SIDE = 5028,
    GLOVE_CT_SIDE = 5029,
    GLOVE_SPORTY = 5030,
    GLOVE_SLICK = 5031,
    GLOVE_LEATHER_WRAP = 5032,
    GLOVE_MOTORCYCLE = 5033,
    GLOVE_SPECIALIST = 5034,
    GLOVE_HYDRA = 5035
};

class CBaseWeapon : public CBaseEntity
{
public:
    float getInaccuracy()
    {
        using fn = float(__thiscall*)(void*);
        return (*(fn**)this)[482](this);
    }
    float getSpread()
    {
        using fn = float(__thiscall*)(void*);
        return (*(fn**)this)[452](this);
    }
    float getNextPrimaryAttack()
    {
        return *(float*)((DWORD)this + hazedumper::netvars::m_flNextPrimaryAttack);
    }
    float getNextAttack()
    {
        return *(float*)((DWORD)this + hazedumper::netvars::m_flNextAttack);
    }
    int getClip1Count()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iClip1);
    }
    short getItemDefinitionIndex()
    {
        return *(short*)((DWORD)this + hazedumper::netvars::m_iItemDefinitionIndex);
    }
    void setItemDefinitionIndex(short value)
    {
        *(short*)((DWORD)this + hazedumper::netvars::m_iItemDefinitionIndex) = value;
    }
    int getEntityQuality()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iEntityQuality);
    }
    void setEntityQuality(int value)
    {
        *(int*)((DWORD)this + hazedumper::netvars::m_iEntityQuality) = value;
    }
    int getViewModelIndex()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iViewModelIndex);
    }
    bool getInReload()
    {
        return *(bool*)((DWORD)this + hazedumper::netvars::m_bInReload);
    }
    int setItemIdHigh(int value)
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iItemIDHigh);
    }
};

class CBasePlayer : public CBaseEntity
{
public:
    void setLocalVAngles(Vector& angle)
    {
        using fn = void(__thiscall*)(void*, Vector&);
        return (*(fn**)this)[372](this, angle);
    }
    bool getHasDefuser()
    {
        return *(bool*)((DWORD)this + hazedumper::netvars::m_bHasDefuser);
    }
    bool getGunGameImmunity()
    {
        return *(bool*)((DWORD)this + hazedumper::netvars::m_bGunGameImmunity);
    }
    int getShotsFired()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iShotsFired);
    }
    int getArmorValue()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_ArmorValue);
    }
    bool getHasHelmet()
    {
        return *(bool*)((DWORD)this + hazedumper::netvars::m_bHasHelmet);
    }
    bool getIsScoped()
    {
        return *(bool*)((DWORD)this + hazedumper::netvars::m_bIsScoped);
    }
    bool getIsDefusing()
    {
        return *(bool*)((DWORD)this + hazedumper::netvars::m_bIsDefusing);
    }
    int getAccountID()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iAccountID);
    }
    float getLowerBodyYaw()
    {
        return *(float*)((DWORD)this + hazedumper::netvars::m_flLowerBodyYawTarget);
    }
    float getFlashDuration()
    {
        return *(float*)((DWORD)this + hazedumper::netvars::m_flFlashDuration);
    }
    float getFlashMaxAlpha()
    {
        return *(float*)((DWORD)this + hazedumper::netvars::m_flFlashMaxAlpha);
    }
    int getHealth()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iHealth);
    }
    int getLifeState()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_lifeState);
    }
    Vector getViewPunchAngle()
    {
        return *(Vector*)((DWORD)this + hazedumper::netvars::m_viewPunchAngle);
    }
    Vector getVelocity()
    {
        return *(Vector*)((DWORD)this + hazedumper::netvars::m_vecVelocity);
    }
    DWORD getObserverTarget()
    {
        return *(DWORD*)((DWORD)this + hazedumper::netvars::m_hObserverTarget);
    }
    DWORD getCurrentWeapon()
    {
        return *(DWORD*)((DWORD)this + hazedumper::netvars::m_hActiveWeapon);
    }
    int getTickBase()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_nTickBase);
    }
    int getMoveType()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_MoveType);
    }
    Vector getEyePosition()
    {
        Vector origin = this->getOrigin();
        origin.z += this->getViewOffset().z;
        return origin;
    }
    Vector getBonePosition(int bone)
    {
        DWORD matrix = *(DWORD*)((DWORD)this + hazedumper::netvars::m_dwBoneMatrix);
        return Vector(
            *(float*)(matrix + 0x30 * bone + 0x0C),
            *(float*)(matrix + 0x30 * bone + 0x1C),
            *(float*)(matrix + 0x30 * bone + 0x2C)
        );
    }
    // move to own class later
    Vector getAimPunchAngle()
    {
        return *(Vector*)((DWORD)this + hazedumper::netvars::m_aimPunchAngle);
    }
    int getCrosshairID()
    {
        return *(int*)((DWORD)this + hazedumper::netvars::m_iCrosshairId);
    }
};