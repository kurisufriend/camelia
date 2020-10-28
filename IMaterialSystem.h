#pragma once
#include "includes.h"
class IMaterialSystem
{
public:
	IMaterial* CreateMaterial(bool flat, bool ignorez, bool wireframed)
	{
		return 0;
	}
	IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName = NULL, bool complain = true, const char* pComplainPrefix = NULL)
	{
		using OriginalFn = IMaterial* (__thiscall*)(void*, char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix);
		return (*(OriginalFn**)this)[84](this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
};