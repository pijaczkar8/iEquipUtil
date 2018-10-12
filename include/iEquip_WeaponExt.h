#pragma once

#include "GameObjects.h"  // TESObjectWEAP
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry


namespace iEquip_WeaponExt
{
	bool IsWeaponBound(StaticFunctionTag* a_base, TESObjectWEAP* a_weap);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}