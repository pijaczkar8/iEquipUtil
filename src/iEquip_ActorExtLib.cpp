#include "iEquip_ActorExtLib.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameData.h"  // GetLeftHandSlot, GetRightHandSlot, GetEitherHandSlot
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // AlchemyItem, EnchantmentItem
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "PapyrusActor.h"
#include "IDebugLog.h"  // gLog

#include "iEquip_Utility.h"  // ExtraDataLocator
#include "RE_BaseExtraData.h"  // RE::BaseExtraData
#include "RE_EnchantmentItem.h"  // RE::EnchantmentItem


using iEquip_Utility::ExtraListLocator;


namespace iEquip_ActorExt
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
		case kSlotId_Right:
			return GetRightHandSlot();
		case kSlotId_Left:
			return GetLeftHandSlot();
		default:
			return 0;
		}
	}


	bool CanEquipBothHands(Actor* a_actor, TESForm* a_item)
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


	ActorEquipPoisonedItem::ActorEquipPoisonedItem(AlchemyItem* a_poison) :
		_poison(a_poison)
	{}


	ActorEquipPoisonedItem::~ActorEquipPoisonedItem()
	{}


	bool ActorEquipPoisonedItem::validate()
	{
		if (!_poison) {
			_ERROR("ERROR: In ActorEquipPoisonedItem::validate() : _poison is a NONE form!");
			return false;
		} else if (_poison->formType != kFormType_Potion) {
			_ERROR("ERROR: In ActorEquipPoisonedItem::validate() : _poison is not a potion!");
			return false;
		}
		return true;
	}


	BaseExtraList* ActorEquipPoisonedItem::findExtraListByForm(InventoryEntryData* a_entryData)
	{
		ExtraListLocator extraListLocator(a_entryData, { kExtraData_Poison }, { kExtraData_Enchantment });
		BaseExtraList* extraList = 0;
		while (extraList = extraListLocator.found()) {
			RE::BSExtraData* extraData = reinterpret_cast<RE::BSExtraData*>(extraList->m_data);
			bool extraPoisonFound = false;
			while (extraData) {
				if (extraData->form->formID == _poison->formID) {
					extraPoisonFound = true;
					break;
				}
				extraData = extraData->next;
			}
			if (extraPoisonFound) {
				return extraList;
			}
		}
		return 0;
	}


	ActorEquipEnchantedItem::ActorEquipEnchantedItem(EnchantmentItem* a_enchantment) :
		_enchantment(a_enchantment)
	{}


	ActorEquipEnchantedItem::~ActorEquipEnchantedItem()
	{}


	bool ActorEquipEnchantedItem::validate()
	{
		if (!_enchantment) {
			_ERROR("ERROR: In ActorEquipEnchantedItem::validate() : _enchantment is a NONE form!");
			return false;
		} if (_enchantment->formType != kFormType_Enchantment) {
			_ERROR("ERROR: In ActorEquipEnchantedItem::validate() : _enchantment is not an enchantment!");
			return false;
		}
		return true;
	}


	BaseExtraList* ActorEquipEnchantedItem::findExtraListByForm(InventoryEntryData* a_entryData)
	{
		ExtraListLocator extraListLocator(a_entryData, { kExtraData_Enchantment }, { kExtraData_Poison });
		BaseExtraList* extraList = 0;
		while (extraList = extraListLocator.found()) {
			RE::BSExtraData* extraData = reinterpret_cast<RE::BSExtraData*>(extraList->m_data);
			bool extraEnchantmentFound = false;
			while (extraData) {
				if (extraData->form->formID == _enchantment->formID) {
					extraEnchantmentFound = true;
					break;
				}
				extraData = extraData->next;
			}
			if (extraEnchantmentFound) {
				return extraList;
			}
		}
		return 0;
	}


	ActorEquipPoisonedAndEnchantedItem::ActorEquipPoisonedAndEnchantedItem(AlchemyItem* a_poison, EnchantmentItem* a_enchantment) :
		ActorEquipPoisonedItem(a_poison),
		ActorEquipEnchantedItem(a_enchantment)
	{}


	ActorEquipPoisonedAndEnchantedItem::~ActorEquipPoisonedAndEnchantedItem()
	{}


	bool ActorEquipPoisonedAndEnchantedItem::validate()
	{
		return (ActorEquipPoisonedItem::validate() && ActorEquipEnchantedItem::validate());
	}


	BaseExtraList* ActorEquipPoisonedAndEnchantedItem::findExtraListByForm(InventoryEntryData* a_entryData)
	{
		ExtraListLocator extraListLocator(a_entryData, { kExtraData_Poison, kExtraData_Enchantment }, { });
		BaseExtraList* extraList = 0;
		while (extraList = extraListLocator.found()) {
			RE::BSExtraData* extraData = reinterpret_cast<RE::BSExtraData*>(extraList->m_data);
			bool extraPoisonFound = false;
			bool extraEnchantmentFound = false;
			while (extraData) {
				if (extraData->form->formType == kFormType_Potion) {
					if (extraData->form->formID == _poison->formID) {
						extraPoisonFound = true;
					}
				} else if (extraData->form->formType == kFormType_Enchantment) {
					ExtraEnchantment* extraEnchant = reinterpret_cast<ExtraEnchantment*>(extraData);
					if (extraEnchant && extraEnchant->enchant == _enchantment) {
						extraEnchantmentFound = true;
					}
				}
				extraData = extraData->next;
			}
			if (extraPoisonFound && extraEnchantmentFound) {
				return extraList;
			}
		}
		return 0;
	}
}