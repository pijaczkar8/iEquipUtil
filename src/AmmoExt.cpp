#include "AmmoExt.h"

#include "GameObjects.h"  // TESAmmo
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction

#include "Utility.h"  // WeapTypeBoundArrow


namespace iEquip
{
	bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			_ERROR("[ERROR] Invalid ammo!");
			return false;
		} else {
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
}
