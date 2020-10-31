#include "knifechanger.h"
#include "globals.h"
#include "settings.h"

#define BAYONET_PRECACHE 89
#define BAYONET_T_PRECACHE 64

void modules::knifechanger()
{
	if (!g::pentLocalPlayer)
		return;
	if (!settings::bKnifechanger)
		return;
	CBaseWeapon* weapon = (CBaseWeapon*)interfaces::pacClientEntityList->GetClientEntityFromHandle(g::pentLocalPlayer->getCurrentWeapon());
	if (!weapon)
		return;
	int viewModelIndex = weapon->getViewModelIndex();
	if (!viewModelIndex)
		return;
	CBaseEntity* viewmodel = interfaces::pacClientEntityList->GetClientEntityFromHandle(g::pentLocalPlayer->getViewmodel());
	if (!viewmodel)
		return;
	int desiredModelIndex = settings::iKnife;
	int modelIndexOffset = (desiredModelIndex < 11) ? 1 : 2; // new knives
	if (weapon->getItemDefinitionIndex() == WEAPON_KNIFE && viewModelIndex > 0)
	{
		viewmodel->setModelIndexVFunc(viewModelIndex + BAYONET_PRECACHE + 3 * desiredModelIndex + modelIndexOffset);
	}
	else if (weapon->getItemDefinitionIndex() == WEAPON_KNIFE_T && viewModelIndex > 0)
	{
		viewmodel->setModelIndexVFunc(viewModelIndex + BAYONET_T_PRECACHE + 3 * desiredModelIndex + modelIndexOffset);
	}
}