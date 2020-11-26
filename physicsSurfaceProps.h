#pragma once
#include "miscshit.h"
class PhysicsSurfaceProps
{
public:
    surfacegameprops_t* getSurfaceData(int index)
    {
        using originalFn = surfacegameprops_t*(__thiscall*)(PhysicsSurfaceProps*, int);
        return (*(originalFn**)this)[5](this, index);
    }
};