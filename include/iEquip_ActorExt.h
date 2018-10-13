#pragma once

#include "ITypes.h"  // SInt32
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // TESAmmo
#include "GameReferences.h"  // Actor
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry

#include "iEquip_ActorExtLib.h"  // IActorEquipItem


namespace iEquip_ActorExt
{
	TESAmmo* GetEquippedAmmo(StaticFunctionTag* a_base, Actor* a_actor);
	void EquipEnchantedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, bool a_preventUnequip = false, bool a_equipSound = true);
	void EquipPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, AlchemyItem* a_poison, UInt32 a_count = 1, bool a_preventUnequip = false, bool a_equipSound = true);
	void EquipEnchantedAndPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, AlchemyItem* a_poison, UInt32 a_count = 1, bool a_preventUnequip = false, bool a_equipSound = true);
	void EquipItemEx(Actor* a_actor, TESForm* a_item, SInt32 a_slotID, IActorEquipItem* a_iActorEquipItem, bool a_preventUnequip, bool a_equipSound);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}