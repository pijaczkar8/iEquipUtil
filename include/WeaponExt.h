#pragma once

#include "GameObjects.h"  // TESObjectWEAP
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry


namespace iEquip
{
	bool IsWeaponBound(StaticFunctionTag* a_base, TESObjectWEAP* a_weap);
	bool IsWeaponGrenade(StaticFunctionTag* a_base, TESObjectWEAP* a_weap);


	namespace WeaponExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}