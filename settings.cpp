#include "settings.h"

bool settings::bESP = false;
bool settings::bESPOnVisible = false;
bool settings::bHealthESP = false;
bool settings::bNameESP = false;
bool settings::bBoxESP = false;
bool settings::bGlowESP = false;
bool settings::bPlayerChams = false;
bool settings::bArmChams = false;
bool settings::bRemoveArms = false;
bool settings::bNightmode = false;
bool settings::bForceCrosshair = false;
bool settings::bRecoilCrosshair = false;
bool settings::bRemoveScopeOverlay = false;
int settings::iNightmodeLevel = 0;
bool settings::bGrenadePrediction = false;

bool settings::bAntiAim = false;
int settings::iAAX = 0;
int settings::iAAY = 0; // lol // nvm used to be flAAY
bool settings::bJitter = false;
int settings::iJitterAmount = 0;
bool settings::bFakelag = false;
int settings::iFakelagAmount = 0;

bool settings::bAimbot;
bool settings::bSilentAimbot;
int settings::iAimbotFOV;
bool settings::bAimbotSmoothing;
int settings::iSmoothingAmount;
bool settings::bAutoShoot;
bool settings::bAutoStop;
int settings::iMinimumHitchance;

bool settings::bTriggerbot;

bool settings::bAutohop;
bool settings::bSlowWalk;
int settings::iSlowWalkAmount;

bool settings::bAngleFix = false;
bool settings::bThirdPerson = false;