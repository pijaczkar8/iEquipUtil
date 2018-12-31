#pragma once

class Actor;
class TESObjectWEAP;
class VMClassRegistry;
struct StaticFunctionTag;


namespace iEquip
{
	enum
	{
		kHand_Unequipped,
		kHand_Right,
		kHand_Left,
		kHand_DualWield
	};


	TESObjectWEAP*	GetEquippedWeapon(StaticFunctionTag* a_base, Actor* a_actor, SInt32 a_hand);
	bool			IsWeaponBound(StaticFunctionTag* a_base, TESObjectWEAP* a_weap);


	namespace WeaponExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}
