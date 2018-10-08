#include "iEquip_ActorExtLib.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // AlchemyItem, EnchantmentItem
#include "IDebugLog.h"  // gLog

#include "RE_BaseExtraData.h"  // RE::BaseExtraData
#include "RE_EnchantmentItem.h"  // RE::EnchantmentItem


#include <string>  // string
#include <sstream>  // stringstream


namespace iEquip_ActorExt
{
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
		BaseExtraList* extraList = 0;
		for (UInt32 i = 0; i < a_entryData->extendDataList->Count(); ++i) {
			extraList = a_entryData->extendDataList->GetNthItem(i);
			if (extraList && extraList->HasType(kExtraData_Poison) && !extraList->HasType(kExtraData_Enchantment)) {
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
		BaseExtraList* extraList = 0;
		for (UInt32 i = 0; i < a_entryData->extendDataList->Count(); ++i) {
			extraList = a_entryData->extendDataList->GetNthItem(i);
			if (extraList && extraList->HasType(kExtraData_Enchantment) && !extraList->HasType(kExtraData_Poison)) {
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
		BaseExtraList* extraList = 0;
		for (UInt32 i = 0; i < a_entryData->extendDataList->Count(); ++i) {
			extraList = a_entryData->extendDataList->GetNthItem(i);
			if (extraList && extraList->HasType(kExtraData_Poison) && extraList->HasType(kExtraData_Enchantment)) {
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
		}
		return 0;
	}
}