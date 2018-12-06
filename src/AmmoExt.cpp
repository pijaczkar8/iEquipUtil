#include "AmmoExt.h"

#include "GameForms.h"  // BGSKeyword, BGSDefaultObjectManager
#include "GameObjects.h"  // TESAmmo
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag

#include "Utility.h"


namespace iEquip
{
	bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			_ERROR("[ERROR] In IsAmmoBound() : Invalid ammo!");
			return false;
		} else {
			BGSKeyword* weapTypeBoundArrow = static_cast<BGSKeyword*>(BGSDefaultObjectManager::GetSingleton()->objects[0x116]);
			return a_ammo->keyword.HasKeyword(weapTypeBoundArrow);
		}
	}


	bool IsAmmoJavelin(StaticFunctionTag* a_base, TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			_ERROR("[ERROR] In IsAmmoJavelin() : Invalid ammo!");
			return false;
		}

		BGSKeyword* Javelin = GetKeyword(kKeyword_Javelin, NAME_SpearsBySoolie);
		if (!Javelin) {
			_WARNING("[WARNING] In IsAmmoJavelin() : Failed to retrieve Javelin keyword!");
			return false;
		}

		return a_ammo->keyword.HasKeyword(Javelin);
	}


	namespace AmmoExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESAmmo*>("IsAmmoBound", "iEquip_AmmoExt", IsAmmoBound, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESAmmo*>("IsAmmoJavelin", "iEquip_AmmoExt", IsAmmoJavelin, a_registry));

			return true;
		}
	}
}
