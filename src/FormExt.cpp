#include "FormExt.h"

#include "GameForms.h"  // TESForm
#include "GameTypes.h"  // BSFixedString
#include "PapyrusEvents.h"  // RegistrationMapHolder, ModCallbackParameters
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <vector>  // vector
#include <string>  // string

#include "Events.h"  // g_callbackRegs
#include "Utility.h"  // iEquip_Utility::numToHexString()


namespace iEquip
{
	void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("[ERROR] In RegisterForBoundWeaponEquippedEvent() : Invalid form!\n");
			return;
		} else {
			g_boundWeaponEquippedCallbackRegs.Register<TESForm>(a_thisForm->GetFormType(), a_thisForm);
			_DMESSAGE("[DEBUG] Registered (0x%X) for OnBoundWeaponEquippedEvent\n", a_thisForm->formID);
		}
	}


	void RegisterForBoundWeaponUnequippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("[ERROR] In RegisterForBoundWeaponUnequippedEvent() : Invalid form!\n");
			return;
		} else {
			g_boundWeaponUnequippedCallbackRegs.Register<TESForm>(a_thisForm->GetFormType(), a_thisForm);
			_DMESSAGE("[DEBUG] Registered (0x%X) for OnBoundWeaponUnequippedEvent\n", a_thisForm->formID);
		}
	}


	namespace FormExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponEquippedEvent", "iEquip_FormExt", RegisterForBoundWeaponEquippedEvent, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponUnequippedEvent", "iEquip_FormExt", RegisterForBoundWeaponUnequippedEvent, a_registry));

			return true;
		}
	}
}
