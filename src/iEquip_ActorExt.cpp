#include "iEquip_ActorExt.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameData.h"  // EquipManager
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData, ExtraPoison
#include "GameForms.h"  // TESForm, BGSEquipSlot
#include "GameObjects.h"  // AlchemyItem
#include "GameReferences.h"  // Actor
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "IDebugLog.h"  // gLog
#include "ITypes.h"  // SInt32
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry
#include "Utilities.h"  // CALL_MEMBER_FN

#include "iEquip_Utility.h"  // numToHexString


namespace iEquip_ActorExt
{
	void EquipItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, TESForm* a_extraForm, bool a_preventUnequip, bool a_equipSound)
	{
		if (!a_item || !a_item->Has3D()) {
			_ERROR("ERROR: In EquipItemEx() : Invalid form!");
			return;
		}

		if (!a_extraForm) {
			_ERROR("ERROR: In EquipItemEx() : Invalid extra form!");
			return;
		}

		EquipManager* equipManager = EquipManager::GetSingleton();
		if (!equipManager) {
			_ERROR("ERROR: In EquipItemEx() : EquipManager not found!");
			return;
		}

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(a_actor->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : 0;
		if (!containerData) {
			_ERROR("ERROR: In EquipItemEx() : No container data!");
			return;
		}

		// Copy/merge of extraData and container base. Free after use.
		InventoryEntryData* entryData = containerData->CreateEquipEntryData(a_item);
		if (!entryData) {
			_ERROR("ERROR: In EquipItemEx() : No entry data!");
			return;
		}

		BGSEquipSlot* targetEquipSlot = GetEquipSlotByID(a_slotID);

		SInt32 itemCount = entryData->countDelta;

		// For ammo, use count, otherwise always equip 1
		SInt32 equipCount = a_item->IsAmmo() ? itemCount : 1;

		bool isTargetSlotInUse = false;

		bool hasItemMinCount = itemCount > 0;

		BaseExtraList* rightEquipList = 0;
		BaseExtraList* leftEquipList = 0;

		BaseExtraList* curEquipList = 0;
		BaseExtraList* extraList = 0;

		if (hasItemMinCount) {
			entryData->GetExtraWornBaseLists(&rightEquipList, &leftEquipList);

			// Case 1: Type already equipped in both hands.
			if (leftEquipList && rightEquipList) {
				isTargetSlotInUse = true;
				curEquipList = (targetEquipSlot == GetLeftHandSlot()) ? leftEquipList : rightEquipList;
				extraList = 0;
			}
			// Case 2: Type already equipped in right hand.
			else if (rightEquipList) {
				isTargetSlotInUse = targetEquipSlot == GetRightHandSlot();
				curEquipList = rightEquipList;
				extraList = 0;
			}
			// Case 3: Type already equipped in left hand.
			else if (leftEquipList) {
				isTargetSlotInUse = targetEquipSlot == GetLeftHandSlot();
				curEquipList = leftEquipList;
				extraList = 0;
			}
			// Case 4: Type not equipped yet.
			else {
				isTargetSlotInUse = false;
				curEquipList = 0;
				extraList = findExtraListByForm(entryData, a_extraForm);
				if (!extraList) {
					_ERROR("ERROR: In EquipItemEx() : No extra list!");
					return;
				}
			}
		}

		// Free temp equip entryData
		entryData->Delete();

		// Normally EquipManager would update CannotWear, if equip is skipped we do it here
		if (isTargetSlotInUse) {
			BSExtraData* xCannotWear = curEquipList->GetByType(kExtraData_CannotWear);
			if (xCannotWear && !a_preventUnequip) {
				curEquipList->Remove(kExtraData_CannotWear, xCannotWear);
			} else if (!xCannotWear && a_preventUnequip) {
				curEquipList->Add(kExtraData_CannotWear, ExtraCannotWear::Create());
			}

			// Slot in use, nothing left to do
			return;
		}

		// For dual wield, prevent that 1 item can be equipped in two hands if its already equipped
		bool isEquipped = (rightEquipList || leftEquipList);
		if (targetEquipSlot && isEquipped && CanEquipBothHands(a_actor, a_item)) {
			hasItemMinCount = itemCount > 1;
		}

		if (!isTargetSlotInUse && hasItemMinCount) {
			CALL_MEMBER_FN(equipManager, EquipItem)(a_actor, a_item, extraList, equipCount, targetEquipSlot, a_equipSound, a_preventUnequip, false, 0);
		}
	}


	BGSEquipSlot* GetEquipSlotByID(SInt32 a_slotID)
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


	BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData, TESForm* a_extraForm)
	{
		// Validate form type
		UInt32 type;
		switch (a_extraForm->formType) {
		case kFormType_Potion:
			type = kExtraData_Poison;
			break;
		case kFormType_Enchantment:
			type = kExtraData_Enchantment;
			break;
		default:
			_ERROR("ERROR: In findExtraListByForm() : Invalid form type!");
			return 0;
		}

		// Locate correct extra list
		BaseExtraList* extraList = 0;
		for (UInt32 i = 0; i < a_entryData->extendDataList->Count(); ++i) {
			extraList = a_entryData->extendDataList->GetNthItem(i);
			if (extraList && extraList->HasType(type)) {
				if (type == kExtraData_Poison) {
					ExtraPoison* extraPoison = reinterpret_cast<ExtraPoison*>(extraList->m_data);
					if (extraPoison->poison->formID == a_extraForm->formID) {
						break;
					}
				} else if (type == kExtraData_Enchantment) {
					ExtraEnchantment* extraEnchant = reinterpret_cast<ExtraEnchantment*>(extraList->m_data);
					if (extraEnchant->enchant->formID == a_extraForm->formID) {
						break;
					}
				}
			}
		}
		return extraList;
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


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction6<StaticFunctionTag, void, Actor*, TESForm*, SInt32, TESForm*, bool, bool>("EquipItemEx", "iEquip_ActorExt", iEquip_ActorExt::EquipItemEx, a_registry));

		return true;
	}
}