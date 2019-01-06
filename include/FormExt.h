#pragma once

class TESForm;
class VMClassRegistry;
struct StaticFunctionTag;


namespace iEquip
{
	void	RegisterForBoundWeaponEquippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);
	void	RegisterForBoundWeaponUnequippedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);
	bool	IsSpear(StaticFunctionTag* a_base, TESForm* a_form);
	bool	IsJavelin(StaticFunctionTag* a_base, TESForm* a_form);
	bool	IsGrenade(StaticFunctionTag* a_base, TESForm* a_form);
	bool	IsThrowingAxe(StaticFunctionTag* a_base, TESForm* a_form);
	bool	IsThrowingKnife(StaticFunctionTag* a_base, TESForm* a_form);
	bool	IsPoisonWax(StaticFunctionTag* a_base, TESForm* a_form);
	bool	IsPoisonOil(StaticFunctionTag* a_base, TESForm* a_form);
	bool	IsSpellWard(StaticFunctionTag* a_base, TESForm* a_form);
	bool	HasFire(StaticFunctionTag* a_base, TESForm* a_form);
	bool	HasIce(StaticFunctionTag* a_base, TESForm* a_form);
	bool	HasShock(StaticFunctionTag* a_base, TESForm* a_form);
	bool	HasPoison(StaticFunctionTag* a_base, TESForm* a_form);


	namespace FormExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}
