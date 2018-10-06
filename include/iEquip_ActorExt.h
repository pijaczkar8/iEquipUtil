#pragma once

#include "ITypes.h"  // SInt32
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameReferences.h"  // Actor
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry


namespace iEquip_ActorExt
{
	enum
	{
		kSlotId_Default = 0,
		kSlotId_Right = 1,
		kSlotId_Left = 2
	};


	void EquipItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, TESForm* a_extraForm, bool a_preventUnequip, bool a_equipSound);
	BGSEquipSlot* GetEquipSlotByID(SInt32 a_slotID);
	BaseExtraList* findExtraListByForm(InventoryEntryData* a_entryData, TESForm* a_extraForm);
	bool CanEquipBothHands(Actor* a_actor, TESForm* a_item);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}