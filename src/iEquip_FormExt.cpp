#include "iEquip_FormExt.h"

#include "GameForms.h"  // TESForm
#include "GameTypes.h"  // BSFixedString
#include "PapyrusEvents.h"  // ModCallbackParameters
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <vector>  // vector


#include <sstream>  // TODO


namespace iEquip_FormExt
{
	std::vector<TESForm*> registry;


	void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("ERROR: In iEquip_FormExt::RegisterForBoundWeaponEquippedEvent() : Invalid form!");
			return;
		} else {
			// TODO
			std::stringstream ss;
			ss << a_thisForm;
			_DMESSAGE(ss.str().c_str());
			// TODO

			registry.push_back(a_thisForm);

			static BSFixedString eventName = "BoundWeaponEquipped";
			static BSFixedString callbackName = "OnBoundWeaponEquipped";
			ModCallbackParameters params;
			params.callbackName = callbackName;

			g_modCallbackRegs.Register<TESForm>(eventName, a_thisForm->GetFormType(), a_thisForm, &params);
		}
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponEquippedEvent", "iEquip_FormExt", iEquip_FormExt::RegisterForBoundWeaponEquippedEvent, a_registry));

		return true;
	}
}