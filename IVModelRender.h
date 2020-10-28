#pragma once
#include "utils.h"
#include "sdk.h"
class IVModelRender
{
public:
    virtual int DrawModel(int flags, void* pRenderable, uintptr_t instance, int entity_index,
        const model_t* model, Vector const& origin, Vector const& angles, int skin, int body,
        int hitboxset, const void* modelToWorld = NULL,
        const void* pLightingOffset = NULL) = 0;

    virtual void ForcedMaterialOverride(IMaterial* newMaterial, int nOverrideType = 0, int nOverrides = 0) = 0;
    virtual bool IsForcedMaterialOverride(void) = 0;
    virtual void SetViewTarget(const int* pStudioHdr, int nBodyIndex, const Vector& target) = 0;
    virtual uintptr_t CreateInstance(void* pRenderable, void* pCache = NULL) = 0;
    virtual void DestroyInstance(uintptr_t handle) = 0;
};