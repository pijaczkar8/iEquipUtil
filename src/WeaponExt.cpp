#include "WeaponExt.h"

#include "GameObjects.h"  // TESObjectWEAP
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include "Utility.h"


namespace iEquip
{
	bool IsWeaponBound(StaticFunctionTag* a_base, TESObjectWEAP* a_weap)
	{
		if (!a_weap) {
			_ERROR("[ERROR] In IsWeaponBound() : Invalid weapon!");
			return false;
		}

		return ((a_weap->gameData.flags1 & TESObjectWEAP::GameData::kFlags_BoundWeapon) == TESObjectWEAP::GameData::kFlags_BoundWeapon);
	}


	bool IsWeaponGrenade(StaticFunctionTag* a_base, TESObjectWEAP* a_weap)
	{
		if (!a_weap) {
			_ERROR("[ERROR] In IsWeaponGrenade() : Invalid weapon!");
			return false;
		}

		BGSKeyword* WAF_WeapTypeGrenade = GetKeyword(kKeyword_WAF_WeapTypeGrenade, NAME_Update);
		if (!WAF_WeapTypeGrenade) {
			_WARNING("[WARNING] In IsWeaponGrenade() : Failed to retrieve WAF_WeapTypeGrenade keyword!");
			return false;
		}

		return a_weap->keyword.HasKeyword(WAF_WeapTypeGrenade);
	}


	namespace WeaponExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESObjectWEAP*>("IsWeaponBound", "iEquip_WeaponExt", IsWeaponBound, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESObjectWEAP*>("IsWeaponGrenade", "iEquip_WeaponExt", IsWeaponGrenade, a_registry));

			return true;
		}
	}
}