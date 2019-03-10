#include "AmmoExt.h"

#include "GameForms.h"  // BGSKeyword, LookupFormByID
#include "GameObjects.h"  // TESAmmo
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction


bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo)
{
	if (!a_ammo) {
		_ERROR("[ERROR] Invalid ammo!");
		return false;
	} else {
		BGSKeyword* WeapTypeBoundArrow = static_cast<BGSKeyword*>(LookupFormByID(0x0010D501));
		return a_ammo->keyword.HasKeyword(WeapTypeBoundArrow);
	}
}


namespace AmmoExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, TESAmmo*>("IsAmmoBound", "iEquip_AmmoExt", IsAmmoBound, a_registry));

		return true;
	}
}
