#pragma once

class Actor;
class TESObjectWEAP;
class VMClassRegistry;
struct StaticFunctionTag;


namespace WeaponExt
{
	enum
	{
		kHand_Unequipped,
		kHand_Right,
		kHand_Left,
		kHand_DualWield
	};


	TESObjectWEAP*	GetEquippedWeapon(StaticFunctionTag*, Actor* a_actor, SInt32 a_hand);
	bool			IsWeaponBound(StaticFunctionTag*, TESObjectWEAP* a_weap);
	bool			RegisterFuncs(VMClassRegistry* a_registry);
}
