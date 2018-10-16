#pragma once

#include "GameObjects.h"  // TESAmmo
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry


namespace iEquip_AmmoExt
{
	bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}