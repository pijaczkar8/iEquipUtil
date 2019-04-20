#include "AmmoExt.h"

#include "GameForms.h"  // BGSKeyword, LookupFormByID
#include "GameObjects.h"  // TESAmmo
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction


bool IsAmmoBound(StaticFunctionTag*, TESAmmo* a_ammo)
{
	if (!a_ammo) {
		_WARNING("[ERROR] a_ammo is a NONE form!");
		return false;
	}

	auto WeapTypeBoundArrow = static_cast<BGSKeyword*>(LookupFormByID(0x0010D501));
	return a_ammo->keyword.HasKeyword(WeapTypeBoundArrow);
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
