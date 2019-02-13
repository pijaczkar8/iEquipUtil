#include "ActorExt.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameData.h"  // EquipManager
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm, BGSEquipSlot
#include "GameObjects.h"  // AlchemyItem, TESAmmo
#include "GameReferences.h"  // Actor
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include "ActorExtLib.h"  // IActorEquipItem
#include "ExtraLocator.h"  // ExtraListLocator


namespace iEquip
{
	TESAmmo* GetEquippedAmmo(StaticFunctionTag* a_base, Actor* a_actor)
	{
		if (!a_actor) {
			_ERROR("[ERROR] Invalid actor!");
			return 0;
		}

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(a_actor->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : 0;
		if (!containerData) {
			_ERROR("[ERROR] No container data!");
			return 0;
		}

		InventoryEntryData* entryData = 0;
		BaseExtraList* xList = 0;
		ExtraListLocator xListLocator(0, { kExtraData_Worn }, { });
		for (UInt32 i = 0; i < containerData->objList->Count(); ++i) {
			entryData = containerData->objList->GetNthItem(i);
			if (entryData && entryData->type->IsAmmo()) {
				xListLocator.setEntryData(entryData);
				if (xList = xListLocator.found()) {
					return static_cast<TESAmmo*>(entryData->type);
				}
			}
		}

		return 0;
	}


	void EquipEnchantedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, bool a_preventUnequip, bool a_equipSound)
	{
		ActorEquipEnchantedItem equipEnch(a_enchantment);
		EquipItemEx(a_actor, a_item, a_slotID, &equipEnch, a_preventUnequip, a_equipSound);
	}


	void EquipPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, AlchemyItem* a_poison, UInt32 a_count, bool a_preventUnequip, bool a_equipSound)
	{
		ActorEquipPoisonedItem equipPoison(a_poison, a_count);
		EquipItemEx(a_actor, a_item, a_slotID, &equipPoison, a_preventUnequip, a_equipSound);
	}


	void EquipEnchantedAndPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, AlchemyItem* a_poison, UInt32 a_count, bool a_preventUnequip, bool a_equipSound)
	{
		ActorEquipEnchantedAndPoisonedItem equipEnchAndPoison(a_enchantment, a_poison, a_count);
		EquipItemEx(a_actor, a_item, a_slotID, &equipEnchAndPoison, a_preventUnequip, a_equipSound);
	}


	float GetAVDamage(StaticFunctionTag* a_base, Actor* a_actor, UInt32 a_actorValue)
	{
		if (!a_actor) {
			_ERROR("[ERROR] a_actor is a NONE form!\n");
			return 0.0;
		} else if (a_actorValue > 163) {
			_ERROR("[ERROR] a_actorValue is out of range!\n");
			return 0.0;
		}

		return a_actor->actorValueOwner.GetMaximum(a_actorValue) - a_actor->actorValueOwner.GetCurrent(a_actorValue);
	}


	TESRace* GetBaseRace(StaticFunctionTag* a_base, Actor* a_actor)
	{
		if (!a_actor) {
			_ERROR("[ERROR] a_actor is a NONE form!\n");
			return 0;
		}

		if (a_actor->formID == (*g_thePlayer)->formID) {
			return (*g_thePlayer)->PlayerCharacter::race;
		} else {
			return static_cast<TESNPC*>(a_actor->baseForm)->race.race;
		}
	}


	namespace ActorExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, TESAmmo*, Actor*>("GetEquippedAmmo", "iEquip_ActorExt", GetEquippedAmmo, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction6<StaticFunctionTag, void, Actor*, TESForm*, SInt32, EnchantmentItem*, bool, bool>("EquipEnchantedItemEx", "iEquip_ActorExt", EquipEnchantedItemEx, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction7<StaticFunctionTag, void, Actor*, TESForm*, SInt32, AlchemyItem*, UInt32, bool, bool>("EquipPoisonedItemEx", "iEquip_ActorExt", EquipPoisonedItemEx, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction8<StaticFunctionTag, void, Actor*, TESForm*, SInt32, EnchantmentItem*, AlchemyItem*, UInt32, bool, bool>("EquipEnchantedAndPoisonedItemEx", "iEquip_ActorExt", EquipEnchantedAndPoisonedItemEx, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction2<StaticFunctionTag, float, Actor*, UInt32>("GetAVDamage", "iEquip_ActorExt", GetAVDamage, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, TESRace*, Actor*>("GetBaseRace", "iEquip_ActorExt", GetBaseRace, a_registry));

			return true;
		}
	}
}
