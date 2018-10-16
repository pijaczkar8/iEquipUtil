#include "iEquip_WeaponExt.h"

#include "GameObjects.h"  // TESObjectWEAP
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry


namespace iEquip_WeaponExt
{
	bool IsWeaponBound(StaticFunctionTag* a_base, TESObjectWEAP* a_weap)
	{
		if (!a_weap) {
			_ERROR("[ERROR] In IsWeaponBound() : Invalid weapon!");
			return false;
		}

		return ((a_weap->gameData.flags1 & TESObjectWEAP::GameData::kFlags_BoundWeapon) == TESObjectWEAP::GameData::kFlags_BoundWeapon);
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, TESObjectWEAP*>("IsWeaponBound", "iEquip_WeaponExt", iEquip_WeaponExt::IsWeaponBound, a_registry));

		return true;
	}
}