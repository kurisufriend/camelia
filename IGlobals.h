#pragma once
#include "includes.h"
struct IGlobals
{
    const float realtime;
    const int framecount;
    const float absoluteFrameTime;
    const std::byte pad[4];
    float currenttime;
    float frametime;
    const int maxClients;
    const int tickCount;
    const float intervalPerTick;
};