#include "iEquip_FormExt.h"

#include "GameForms.h"  // TESForm
#include "GameTypes.h"  // BSFixedString
#include "PapyrusEvents.h"  // RegistrationMapHolder, ModCallbackParameters
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <vector>  // vector
#include <string>  // string

#include "iEquip_Events.h"  // g_callbackRegs
#include "iEquip_Utility.h"  // iEquip_Utility::numToHexString()


namespace iEquip_FormExt
{
	void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("[ERROR] In iEquip_FormExt::RegisterForBoundWeaponEquippedEvent() : Invalid form!\n");
			return;
		} else {
			iEquip_Events::g_boundWeaponEquippedCallbackRegs.Register<TESForm>(a_thisForm->GetFormType(), a_thisForm);
			std::string msg = "[DEBUG] Registered (0x" + iEquip_Utility::numToHexString(a_thisForm->formID) + ") for OnBoundWeaponEquippedEvent\n";
			_DMESSAGE(msg.c_str());
		}
	}


	void RegisterForBoundWeaponUnequippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("[ERROR] In iEquip_FormExt::RegisterForBoundWeaponUnequippedEvent() : Invalid form!\n");
			return;
		} else {
			iEquip_Events::g_boundWeaponUnequippedCallbackRegs.Register<TESForm>(a_thisForm->GetFormType(), a_thisForm);
			std::string msg = "[DEBUG] Registered (0x" + iEquip_Utility::numToHexString(a_thisForm->formID) + ") for OnBoundWeaponUnequippedEvent\n";
			_DMESSAGE(msg.c_str());
		}
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponEquippedEvent", "iEquip_FormExt", iEquip_FormExt::RegisterForBoundWeaponEquippedEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponUnequippedEvent", "iEquip_FormExt", iEquip_FormExt::RegisterForBoundWeaponUnequippedEvent, a_registry));

		return true;
	}
}