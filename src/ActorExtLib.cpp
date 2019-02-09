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
}
