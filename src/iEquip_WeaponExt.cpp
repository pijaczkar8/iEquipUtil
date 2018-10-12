#include "iEquip_WeaponExt.h"

#include "GameObjects.h"  // TESObjectWEAP
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <bitset>  // bitset


namespace iEquip_WeaponExt
{
	bool IsWeaponBound(StaticFunctionTag* a_base, TESObjectWEAP* a_weap)
	{
		if (!a_weap) {
			_ERROR("ERROR: In IsWeaponBound() : Invalid weapon!");
			return false;
		}

		std::bitset<16> bits(a_weap->gameData.flags1);
		return (bits.test(13));
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, TESObjectWEAP*>("IsWeaponBound", "iEquip_WeaponExt", iEquip_WeaponExt::IsWeaponBound, a_registry));

		return true;
	}
}