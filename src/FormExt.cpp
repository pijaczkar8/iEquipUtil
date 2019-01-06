#include "FormExt.h"

#include "GameForms.h"  // TESForm
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include "Events.h"  // g_boundWeaponEquippedCallbackRegs, g_boundWeaponUnequippedCallbackRegs
#include "Settings.h"  // Settings


namespace iEquip
{
	void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("[ERROR] Invalid form!\n");
			return;
		} else {
			g_boundWeaponEquippedCallbackRegs.Register<TESForm>(a_thisForm->GetFormType(), a_thisForm);
			_DMESSAGE("[DEBUG] Registered (0x%X) for OnBoundWeaponEquippedEvent\n", a_thisForm->formID);
		}
	}


	void RegisterForBoundWeaponUnequippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
	{
		if (!a_thisForm) {
			_ERROR("[ERROR] Invalid form!\n");
			return;
		} else {
			g_boundWeaponUnequippedCallbackRegs.Register<TESForm>(a_thisForm->GetFormType(), a_thisForm);
			_DMESSAGE("[DEBUG] Registered (0x%X) for OnBoundWeaponUnequippedEvent\n", a_thisForm->formID);
		}
	}


	template <aSetting<Form*>& arr>
	bool IsT(TESForm* a_form)
	{
		if (!a_form) {
			_ERROR("[ERROR] Invalid form!\n");
			return false;
		} else {
			return arr.find(a_form->formID);
		}
	}


	bool IsSpear(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::spears>(a_form);
	}


	bool IsJavelin(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::javelins>(a_form);
	}


	bool IsGrenade(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::grenades>(a_form);
	}


	bool IsThrowingAxe(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::throwingAxes>(a_form);
	}


	bool IsThrowingKnife(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::throwingKnives>(a_form);
	}


	bool IsPoisonWax(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::poisonWaxes>(a_form);
	}


	bool IsPoisonOil(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::poisonOils>(a_form);
	}


	bool IsSpellWard(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::spellWards>(a_form);
	}


	bool HasFire(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::fire>(a_form);
	}


	bool HasIce(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::ice>(a_form);
	}


	bool HasShock(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::shock>(a_form);
	}


	bool HasPoison(StaticFunctionTag* a_base, TESForm* a_form)
	{
		return IsT<Settings::poison>(a_form);
	}


	namespace FormExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponEquippedEvent", "iEquip_FormExt", RegisterForBoundWeaponEquippedEvent, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponUnequippedEvent", "iEquip_FormExt", RegisterForBoundWeaponUnequippedEvent, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsSpear", "iEquip_FormExt", IsSpear, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsJavelin", "iEquip_FormExt", IsJavelin, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsGrenade", "iEquip_FormExt", IsGrenade, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsThrowingAxe", "iEquip_FormExt", IsThrowingAxe, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsThrowingKnife", "iEquip_FormExt", IsThrowingKnife, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsPoisonWax", "iEquip_FormExt", IsPoisonWax, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsPoisonOil", "iEquip_FormExt", IsPoisonOil, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsSpellWard", "iEquip_FormExt", IsSpellWard, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("HasFire", "iEquip_FormExt", HasFire, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("HasIce", "iEquip_FormExt", HasIce, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("HasShock", "iEquip_FormExt", HasShock, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESForm*>("HasPoison", "iEquip_FormExt", HasPoison, a_registry));

			return true;
		}
	}
}
