#pragma once

#include "GameForms.h"  // TESForm
#include "GameTypes.h"  // BSFixedString
#include "PapyrusEvents.h"  // RegistrationMapHolder, ModCallbackParameters
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry

#include <vector>  // vector


namespace iEquip
{
	void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);


	namespace FormExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}