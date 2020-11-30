#pragma once
#include "sdk.h"
#include "globals.h"

#define TICK_INTERVAL			( interfaces::pacGlobals->intervalPerTick )
#define TIME_TO_TICKS( t )		( (int)( 0.5f + (float)( t ) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL * (float)( t ) )


namespace modules
{
	void backtrack(CUserCmd* cmd);
}