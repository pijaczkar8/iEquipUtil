#include "FormExt.h"

#include "GameForms.h"  // TESForm
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include "Registration.h"  // OnBoundWeaponEquippedRegSet, OnBoundWeaponUnequippedRegSet
#include "Settings.h"  // Settings


void RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnBoundWeaponEquippedRegSet::GetSingleton()->Register<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Registered (0x%08X) for OnBoundWeaponEquippedEvent", a_thisForm->formID);
	}
}


void UnregisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnBoundWeaponEquippedRegSet::GetSingleton()->Unregister<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Unregistered (0x%08X) for OnBoundWeaponEquippedEvent", a_thisForm->formID);
	}
}


void RegisterForBoundWeaponUnequippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnBoundWeaponUnequippedRegSet::GetSingleton()->Register<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Registered (0x%08X) for OnBoundWeaponUnequippedEvent", a_thisForm->formID);
	}
}


void UnregisterForBoundWeaponUnequippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnBoundWeaponUnequippedRegSet::GetSingleton()->Unregister<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Unregistered (0x%08X) for OnBoundWeaponUnequippedEvent", a_thisForm->formID);
	}
}


template <aSetting<Form*>& arr>
bool IsT(TESForm* a_form)
{
	if (!a_form) {
		_ERROR("[ERROR] a_form is a NONE form!\n");
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


bool IsWax(StaticFunctionTag* a_base, TESForm* a_form)
{
	return IsT<Settings::waxes>(a_form);
}


bool IsOil(StaticFunctionTag* a_base, TESForm* a_form)
{
	return IsT<Settings::oils>(a_form);
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


bool IsSalve(StaticFunctionTag* a_base, TESForm* a_form)
{
	return IsT<Settings::salves>(a_form);
}


bool IsBandage(StaticFunctionTag* a_base, TESForm* a_form)
{
	return IsT<Settings::bandages>(a_form);
}


namespace FormExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponEquippedEvent", "iEquip_FormExt", RegisterForBoundWeaponEquippedEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("UnregisterForBoundWeaponEquippedEvent", "iEquip_FormExt", UnregisterForBoundWeaponEquippedEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForBoundWeaponUnequippedEvent", "iEquip_FormExt", RegisterForBoundWeaponUnequippedEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("UnregisterForBoundWeaponUnequippedEvent", "iEquip_FormExt", UnregisterForBoundWeaponUnequippedEvent, a_registry));

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
			new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsWax", "iEquip_FormExt", IsWax, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsOil", "iEquip_FormExt", IsOil, a_registry));

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

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsSalve", "iEquip_FormExt", IsSalve, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, TESForm*>("IsBandage", "iEquip_FormExt", IsBandage, a_registry));

		return true;
	}
}
