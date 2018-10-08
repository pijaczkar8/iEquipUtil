#pragma once

#include "ITypes.h"  // SInt32
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameReferences.h"  // Actor
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry

#include "iEquip_ActorExtLib.h"  // IActorEquipItem


namespace iEquip_ActorExt
{
	enum
	{
		kSlotId_Default = 0,
		kSlotId_Right = 1,
		kSlotId_Left = 2
	};


	EnchantmentItem* GetEnchantment(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item);
	void EquipPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, AlchemyItem* a_poison, bool a_preventUnequip, bool a_equipSound);
	void EquipEnchantedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, bool a_preventUnequip, bool a_equipSound);
	void EquipPoisonedAndEnchantedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, AlchemyItem* a_poison, EnchantmentItem* a_enchantment, bool a_preventUnequip, bool a_equipSound);
	void EquipItemEx(Actor* a_actor, TESForm* a_item, SInt32 a_slotID, IActorEquipItem* a_iActorEquipItem, bool a_preventUnequip, bool a_equipSound);
	InventoryEntryData* findEntryData(ExtraContainerChanges::Data* a_containerData, TESForm* a_item);
	BGSEquipSlot* getEquipSlotByID(SInt32 a_slotID);
	bool CanEquipBothHands(Actor* a_actor, TESForm* a_item);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}