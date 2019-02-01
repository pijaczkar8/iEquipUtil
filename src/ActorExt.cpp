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


	void EquipItemEx(Actor* a_actor, TESForm* a_item, SInt32 a_slotID, IActorEquipItem* a_iActorEquipItem, bool a_preventUnequip, bool a_equipSound)
	{
		if (!a_actor) {
			_ERROR("[ERROR] a_actor is a NONE form!\n");
			return;
		} else if (!a_item || !a_item->Has3D()) {
			_ERROR("[ERROR] a_item is a NONE form!\n");
			return;
		} else if (!a_iActorEquipItem->validate()) {
			_ERROR("[ERROR] Failed validation!\n");
			return;
		}

		EquipManager* equipManager = EquipManager::GetSingleton();
		if (!equipManager) {
			_ERROR("[ERROR] EquipManager not found!\n");
			return;
		}

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(a_actor->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : 0;
		if (!containerData) {
			_ERROR("[ERROR] No container data!\n");
			return;
		}

		// Copy/merge of extraData can fail in edge cases. Obtain it ourselves.
		InventoryEntryData* entryData = findEntryData(containerData, a_item);
		if (!entryData) {
			_ERROR("[ERROR] No entry data!\n");
			return;
		}

		BGSEquipSlot* targetEquipSlot = getEquipSlotByID(a_slotID);

		SInt32 itemCount = entryData->countDelta;

		// For ammo, use count, otherwise always equip 1
		SInt32 equipCount = a_item->IsAmmo() ? itemCount : 1;

		bool isTargetSlotInUse = false;

		bool hasItemMinCount = itemCount > 0;

		BaseExtraList* rightEquipList = 0;
		BaseExtraList* leftEquipList = 0;

		BaseExtraList* curEquipList = 0;
		BaseExtraList* xList = 0;

		if (hasItemMinCount) {
			entryData->GetExtraWornBaseLists(&rightEquipList, &leftEquipList);

			// Case 1: Type already equipped in both hands.
			if (leftEquipList && rightEquipList) {
				isTargetSlotInUse = true;
				curEquipList = (targetEquipSlot == GetLeftHandSlot()) ? leftEquipList : rightEquipList;
			}
			// Case 2: Type already equipped in right hand.
			else if (rightEquipList) {
				isTargetSlotInUse = targetEquipSlot == GetRightHandSlot();
				curEquipList = rightEquipList;
			}
			// Case 3: Type already equipped in left hand.
			else if (leftEquipList) {
				isTargetSlotInUse = targetEquipSlot == GetLeftHandSlot();
				curEquipList = leftEquipList;
			}
			// Case 4: Type not equipped yet.
			else {
				isTargetSlotInUse = false;
				curEquipList = 0;
			}

			xList = a_iActorEquipItem->findExtraListByForm(entryData);
			if (!xList) {
				_ERROR("[ERROR] No extra list!\n");
				return;
			}
		}

		// Normally EquipManager would update CannotWear, if equip is skipped we do it here
		if (isTargetSlotInUse) {
			BSExtraData* xCannotWear = curEquipList->GetByType(kExtraData_CannotWear);
			if (xCannotWear && !a_preventUnequip) {
				curEquipList->Remove(kExtraData_CannotWear, xCannotWear);
			} else if (!xCannotWear && a_preventUnequip) {
				curEquipList->Add(kExtraData_CannotWear, ExtraCannotWear::Create());
			}

			// Slot in use, nothing left to do
			_ERROR("[ERROR] Slot in use!\n");
			return;
		}

		// For dual wield, prevent that 1 item can be equipped in two hands if its already equipped
		bool isEquipped = (rightEquipList || leftEquipList);
		if (targetEquipSlot && isEquipped && canEquipBothHands(a_actor, a_item)) {
			hasItemMinCount = itemCount > 1;
		}

		if (!isTargetSlotInUse && hasItemMinCount) {
			CALL_MEMBER_FN(equipManager, EquipItem)(a_actor, a_item, xList, equipCount, targetEquipSlot, a_equipSound, a_preventUnequip, false, 0);
		} else {
			_ERROR("[ERROR] Item does not have min count!\n");
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

			return true;
		}
	}
}
