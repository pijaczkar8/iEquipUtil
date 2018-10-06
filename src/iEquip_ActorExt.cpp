#include "iEquip_ActorExt.h"

#include "common/ITypes.h"  // SInt32
#include "skse64/GameBSExtraData.h"  // BaseExtraList
#include "skse64/GameData.h"  // EquipManager
#include "skse64/GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData, ExtraPoison
#include "skse64/GameForms.h"  // TESForm, BGSEquipSlot
#include "skse64/GameReferences.h"  // Actor
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "skse64/PapyrusVM.h"  // VMClassRegistry

#include "Utility.h"  // numToHexString


#include <sstream>  // stringstream


namespace iEquip_ActorExt
{
	void EquipPoisonedItemByID(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_itemID, SInt32 a_slotID, bool a_preventUnequip, bool a_equipSound, TESForm* a_poison)
	{
		if (!a_item || !a_item->Has3D() || a_itemID == 0 || a_item->IsAmmo()) {
			_ERROR("ERROR: In EquipPoisonedItemById() : Invalid item!");
			return;
		}

		EquipManager* equipManager = EquipManager::GetSingleton();
		if (!equipManager) {
			_ERROR("ERROR: In EquipPoisonedItemById() : EquipManager not found!");
			return;
		}

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(a_actor->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : 0;
		if (!containerData) {
			_ERROR("ERROR: In EquipPoisonedItemById() : No container data!");
			return;
		}

		InventoryEntryData::EquipData itemData;
		containerData->GetEquipItemData(itemData, a_item, a_itemID);

		BGSEquipSlot* targetEquipSlot = GetEquipSlotByID(a_slotID);
		bool isTargetSlotInUse = false;

		BaseExtraList* extraList = findPoisonedItemByID(containerData, a_itemID, a_poison);
		if (!extraList) {
			_ERROR("ERROR: In EquipPoisonedItemById() : No extra list!");
			return;
		}

		// Case 1: Type already equipped in both hands.
		if (itemData.isTypeWorn && itemData.isTypeWornLeft) {
			isTargetSlotInUse = true;
		}
		// Case 2: Type already equipped in right hand.
		else if (itemData.isTypeWorn) {
			isTargetSlotInUse = (targetEquipSlot == GetRightHandSlot() || targetEquipSlot == 0);
		}
		// Case 3: Type already equipped in left hand.
		else if (itemData.isTypeWornLeft) {
			isTargetSlotInUse = (targetEquipSlot == GetLeftHandSlot());
		}
		// Case 4: Type not equipped yet.
		else {
			isTargetSlotInUse = false;
		}

		if (isTargetSlotInUse) {
			_ERROR("ERROR: In EquipPoisonedItemById() : Slot is in use!");
			return;
		}

		CALL_MEMBER_FN(equipManager, EquipItem)(a_actor, a_item, extraList, 1, targetEquipSlot, a_equipSound, a_preventUnequip, false, 0);
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


	BaseExtraList* findPoisonedItemByID(ExtraContainerChanges::Data* a_containerData, SInt32 a_itemID, TESForm* a_poison)
	{
		InventoryEntryData* entryData = 0;
		BaseExtraList* extraList = 0;
		for (UInt32 i = 0; i < a_containerData->objList->Count(); ++i) {
			entryData = a_containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formID != a_itemID) {
					continue;
				}
				for (UInt32 j = 0; j < entryData->extendDataList->Count(); ++j) {
					extraList = entryData->extendDataList->GetNthItem(j);
					if (extraList && extraList->HasType(kExtraData_Poison)) {
						ExtraPoison* extraPoison = reinterpret_cast<ExtraPoison*>(extraList->m_data);
						if (extraPoison->poison->formID == a_poison->formID) {
							return extraList;
						}
					}
				}
			}
		}
		return 0;
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
			new NativeFunction7<StaticFunctionTag, void, Actor*, TESForm*, SInt32, SInt32, bool, bool, TESForm*>("EquipPoisonedItemByID", "iEquip_ActorExt", iEquip_ActorExt::EquipPoisonedItemByID, a_registry));

		return true;
	}
}