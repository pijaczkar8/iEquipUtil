#pragma once

#include "common/ITypes.h"  // SInt32
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameReferences.h"  // Actor
#include "skse64/PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "skse64/PapyrusVM.h"  // VMClassRegistry


namespace iEquip_ActorExt
{
	enum
	{
		kSlotId_Default = 0,
		kSlotId_Right = 1,
		kSlotId_Left = 2
	};


	void EquipPoisonedItemByID(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_itemID, SInt32 a_slotID, bool a_preventUnequip, bool a_equipSound);
	BGSEquipSlot* GetEquipSlotByID(SInt32 a_slotID);
	BaseExtraList* findPoisonedItemByID(ExtraContainerChanges::Data* a_containerData, SInt32 a_itemID);
	bool CanEquipBothHands(Actor* a_actor, TESForm* a_item);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}