#include "ActorExtLib.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameData.h"  // GetLeftHandSlot, GetRightHandSlot, GetEitherHandSlot
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // AlchemyItem, EnchantmentItem
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "IDebugLog.h"  // gLog

#include "ExtraLocator.h"  // ExtraListLocator
#include "RE/ExtraPoison.h"  // RE::ExtraPoison


namespace iEquip
{
	InventoryEntryData* findEntryData(ExtraContainerChanges::Data* a_containerData, TESForm* a_item)
	{
		InventoryEntryData* entryData = 0;
		for (UInt32 i = 0; i < a_containerData->objList->Count(); ++i) {
			entryData = a_containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formID == a_item->formID) {
					return entryData;
				}
			}
		}
		return 0;
	}


	BGSEquipSlot* getEquipSlotByID(SInt32 a_slotID)
	{
		switch (a_slotID) {
		case kSlotID_Right:
			return GetRightHandSlot();
		case kSlotID_Left:
			return GetLeftHandSlot();
		default:
			return 0;
		}
	}


	bool canEquipBothHands(Actor* a_actor, TESForm* a_item)
	{
		BGSEquipType * equipType = DYNAMIC_CAST(a_item, TESForm, BGSEquipType);
		if (!equipType) {
			return false;
		}

		BGSEquipSlot * equipSlot = equipType->GetEquipSlot();
		if (!equipSlot) {
			return false;
		}

		if (equipSlot == GetEitherHandSlot()) {  // 1H
			return true;
		} else if (equipSlot == GetLeftHandSlot() || equipSlot == GetRightHandSlot()) {  // 2H
			return (a_actor->race->data.raceFlags & TESRace::kRace_CanDualWield) && a_item->IsWeapon();
		}

		return false;
	}


	UInt32 getEquippedSlots(Actor* a_actor, TESObjectWEAP* a_weap)
	{
		if (!a_actor) {
			_ERROR("[ERROR] In GetEquippedHand() : a_actor is a NONE form!\n");
			return kSlotID_Default;
		} else if (!a_weap) {
			_ERROR("[ERROR] In GetEquippedHand() : a_weap is a NONE form!\n");
			return kSlotID_Default;
		}

		TESForm* rightHand = a_actor->processManager->equippedObject[ActorProcessManager::kEquippedHand_Right];
		TESForm* leftHand = a_actor->processManager->equippedObject[ActorProcessManager::kEquippedHand_Left];

		UInt32 slotID = 0;
		if (rightHand && rightHand->formID == a_weap->formID) {
			slotID += kSlotID_Right;
		}
		if (leftHand && leftHand->formID == a_weap->formID) {
			slotID += kSlotID_Left;
		}
		return slotID;
	}


	UInt32 getUnequippedSlots(Actor* a_actor)
	{
		if (!a_actor) {
			_ERROR("[ERROR] In GetEquippedHand() : a_actor is a NONE form!\n");
			return kSlotID_Default;
		}

		UInt32 slotID = !a_actor->processManager->equippedObject[ActorProcessManager::kEquippedHand_Right] ? kSlotID_Right : kSlotID_Default;
		slotID += !a_actor->processManager->equippedObject[ActorProcessManager::kEquippedHand_Left] ? kSlotID_Left : kSlotID_Default;
		return slotID;
	}


	ActorEquipEnchantedItem::ActorEquipEnchantedItem(EnchantmentItem* a_enchantment) :
		_enchantment(a_enchantment)
	{}


	ActorEquipEnchantedItem::~ActorEquipEnchantedItem()
	{}


	bool ActorEquipEnchantedItem::validate()
	{
		if (!_enchantment) {
			_ERROR("[ERROR] _enchantment is a NONE form!\n");
			return false;
		} if (_enchantment->formType != kFormType_Enchantment) {
			_ERROR("[ERROR] _enchantment is not an enchantment!\n");
			return false;
		}
		return true;
	}


	BaseExtraList* ActorEquipEnchantedItem::findExtraListByForm(InventoryEntryData* a_entryData)
	{
		ExtraListLocator xListLocator(a_entryData, { kExtraData_Enchantment }, { kExtraData_Poison, kExtraData_Worn, kExtraData_WornLeft });
		BaseExtraList* xList = 0;
		while (xList = xListLocator.found()) {
			BSExtraData* xData = xList->m_data;
			bool xEnchantmentFound = false;
			while (xData) {
				if (xData->GetType() == kExtraData_Enchantment) {
					ExtraEnchantment* xEnchantment = static_cast<ExtraEnchantment*>(xData);
					if (xEnchantment->enchant->formID == _enchantment->formID) {
						xEnchantmentFound = true;
						break;
					}
				}
				xData = xData->next;
			}
			if (xEnchantmentFound) {
				return xList;
			}
		}
		return 0;
	}


	ActorEquipPoisonedItem::ActorEquipPoisonedItem(AlchemyItem* a_poison, UInt32 a_count) :
		_poison(a_poison),
		_count(a_count)
	{}


	ActorEquipPoisonedItem::~ActorEquipPoisonedItem()
	{}


	bool ActorEquipPoisonedItem::validate()
	{
		if (!_poison) {
			_ERROR("[ERROR] _poison is a NONE form!\n");
			return false;
		} else if (_poison->formType != kFormType_Potion) {
			_ERROR("[ERROR] _poison is not a potion!\n");
			return false;
		}
		return true;
	}


	BaseExtraList* ActorEquipPoisonedItem::findExtraListByForm(InventoryEntryData* a_entryData)
	{
		ExtraListLocator xListLocator(a_entryData, { kExtraData_Poison }, { kExtraData_Enchantment, kExtraData_Worn, kExtraData_WornLeft });
		BaseExtraList* xList = 0;
		while (xList = xListLocator.found()) {
			BSExtraData* xData = xList->m_data;
			bool xPoisonFound = false;
			while (xData) {
				if (xData->GetType() == kExtraData_Poison) {
					RE::ExtraPoison* xPoison = static_cast<RE::ExtraPoison*>(xData);
					if (xPoison->poison->formID == _poison->formID && xPoison->count == _count) {
						xPoisonFound = true;
						break;
					}
				}
				xData = xData->next;
			}
			if (xPoisonFound) {
				return xList;
			}
		}
		return 0;
	}


	ActorEquipEnchantedAndPoisonedItem::ActorEquipEnchantedAndPoisonedItem(EnchantmentItem* a_enchantment, AlchemyItem* a_poison, UInt32 a_count) :
		ActorEquipEnchantedItem(a_enchantment),
		ActorEquipPoisonedItem(a_poison, a_count)
	{}


	ActorEquipEnchantedAndPoisonedItem::~ActorEquipEnchantedAndPoisonedItem()
	{}


	bool ActorEquipEnchantedAndPoisonedItem::validate()
	{
		return (ActorEquipPoisonedItem::validate() && ActorEquipEnchantedItem::validate());
	}


	BaseExtraList* ActorEquipEnchantedAndPoisonedItem::findExtraListByForm(InventoryEntryData* a_entryData)
	{
		ExtraListLocator xListLocator(a_entryData, { kExtraData_Enchantment, kExtraData_Poison }, { kExtraData_Worn, kExtraData_WornLeft });
		BaseExtraList* xList = 0;
		while (xList = xListLocator.found()) {
			BSExtraData* xData = xList->m_data;
			bool xEnchantmentFound = false;
			bool xPoisonFound = false;
			while (xData) {
				if (xData->GetType() == kExtraData_Enchantment) {
					ExtraEnchantment* xEnchantment = static_cast<ExtraEnchantment*>(xData);
					if (xEnchantment->enchant->formID == _enchantment->formID) {
						xEnchantmentFound = true;
					}
				} else if (xData->GetType() == kExtraData_Poison) {
					RE::ExtraPoison* xPoison = static_cast<RE::ExtraPoison*>(xData);
					if (xPoison->poison->formID == _poison->formID && xPoison->count == _count) {
						xPoisonFound = true;
					}
				}
				xData = xData->next;
			}
			if (xPoisonFound && xEnchantmentFound) {
				return xList;
			}
		}
		return 0;
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
}
