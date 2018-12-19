#include "WeaponExt.h"

#include "GameData.h"  // GetLeftHandSlot(), GetRightHandSlot()
#include "GameFormComponents.h"  // ActorProcessManager
#include "GameObjects.h"  // TESObjectWEAP
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include "ActorExtLib.h"
#include "Utility.h"


namespace iEquip
{
	TESObjectWEAP* GetEquippedWeapon(StaticFunctionTag* a_base, Actor* a_actor, SInt32 a_hand)
	{
		if (!a_actor) {
			_ERROR("[ERROR] a_actor is a NONE form!\n");
			return 0;
		}

		switch (a_hand) {
		case kHand_Right:
		{
			TESForm* weap = a_actor->processManager ? a_actor->processManager->equippedObject[ActorProcessManager::kEquippedHand_Right] : 0;
			return DYNAMIC_CAST(weap, TESForm, TESObjectWEAP);
		}
		case kHand_Left:
		{
			TESForm* weap = a_actor->processManager ? a_actor->processManager->equippedObject[ActorProcessManager::kEquippedHand_Left] : 0;
			return DYNAMIC_CAST(weap, TESForm, TESObjectWEAP);
		}
		default:
			_ERROR("[ERROR] Invalid slot (%i)!\n", a_hand);
			return 0;
		}
	}


	bool IsWeaponBound(StaticFunctionTag* a_base, TESObjectWEAP* a_weap)
	{
		if (!a_weap) {
			_ERROR("[ERROR] Invalid weapon!");
			return false;
		}

		return ((a_weap->gameData.flags1 & TESObjectWEAP::GameData::kFlags_BoundWeapon) == TESObjectWEAP::GameData::kFlags_BoundWeapon);
	}


	bool IsWeaponGrenade(StaticFunctionTag* a_base, TESObjectWEAP* a_weap)
	{
		if (!a_weap) {
			_ERROR("[ERROR] Invalid weapon!");
			return false;
		}

		BGSKeyword* WAF_WeapTypeGrenade = GetForm<BGSKeyword>(kUpdate_WAF_WeapTypeGrenade, NAME_Update, false);
		if (!WAF_WeapTypeGrenade) {
			_WARNING("[WARNING] Failed to retrieve WAF_WeapTypeGrenade keyword!");
			return false;
		}

		return a_weap->keyword.HasKeyword(WAF_WeapTypeGrenade);
	}


	namespace WeaponExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction2<StaticFunctionTag, TESObjectWEAP*, Actor*, SInt32>("GetEquippedWeapon", "iEquip_WeaponExt", GetEquippedWeapon, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESObjectWEAP*>("IsWeaponBound", "iEquip_WeaponExt", IsWeaponBound, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESObjectWEAP*>("IsWeaponGrenade", "iEquip_WeaponExt", IsWeaponGrenade, a_registry));

			return true;
		}
	}
}
