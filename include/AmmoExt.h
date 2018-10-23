#pragma once

#include "GameObjects.h"  // TESAmmo
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry


namespace iEquip
{
	bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo);
	bool IsAmmoJavelin(StaticFunctionTag* a_base, TESAmmo* a_ammo);


	namespace AmmoExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}