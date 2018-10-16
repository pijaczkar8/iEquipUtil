#include "iEquip_FormExt.h"

#include "GameForms.h"  // TESForm
#include "GameTypes.h"  // BSFixedString
#include "PapyrusEvents.h"  // RegistrationMapHolder, ModCallbackParameters
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <vector>  // vector

#include "iEquip_Events.h"  // g_callbackRegs


namespace iEquip_FormExt
{
	void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("[ERROR] In iEquip_FormExt::RegisterForBoundWeaponEquippedEvent() : Invalid form!");
			return;
		} else {
			iEquip_Events::g_callbackRegs.Register<TESForm>(a_thisForm->GetFormType(), a_thisForm);
			_DMESSAGE("[DEBUG] Registered for OnBoundWeaponEquippedEvent");
		}
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponEquippedEvent", "iEquip_FormExt", iEquip_FormExt::RegisterForBoundWeaponEquippedEvent, a_registry));

		return true;
	}
}