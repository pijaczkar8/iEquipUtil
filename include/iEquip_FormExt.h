#pragma once

#include "GameForms.h"  // TESForm
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry

#include <vector>  // vector


namespace iEquip_FormExt
{
	extern std::vector<TESForm*> registry;


	void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}