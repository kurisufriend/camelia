#pragma once

//forward declare
class CBaseEntity;

// source sdk 2013
class IClientEntityList
{
public:
	// Get IClientNetworkable interface for specified entity
	virtual void * GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableFromHandle(int hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(int hEnt) = 0;

	// NOTE: This function is only a convenience wrapper.
	// It returns GetClientNetworkable( entnum )->GetIClientEntity().
	virtual CBaseEntity* GetClientEntity(int entnum) = 0;
	virtual CBaseEntity* GetClientEntityFromHandle(int hEnt) = 0;

	// Returns number of entities currently in use
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

	// Returns highest index actually used
	virtual int					GetHighestEntityIndex(void) = 0;

	// Sizes entity list to specified size
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};
