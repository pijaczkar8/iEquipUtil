#include "iEquip_AmmoExt.h"

#include "GameForms.h"  // BGSDefaultObjectManager
#include "GameObjects.h"  // TESAmmo
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag


namespace iEquip_AmmoExt
{
	bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			_ERROR("[ERROR] In iEquip_AmmoExt::IsAmmoBound() : Invalid ammo!");
			return false;
		} else {
			static BGSKeyword* weapTypeBoundArrow = static_cast<BGSKeyword*>(BGSDefaultObjectManager::GetSingleton()->objects[0x116]);
			return a_ammo->keyword.HasKeyword(weapTypeBoundArrow);
		}
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, TESAmmo*>("IsAmmoBound", "iEquip_AmmoExt", iEquip_AmmoExt::IsAmmoBound, a_registry));

		return true;
	}
}