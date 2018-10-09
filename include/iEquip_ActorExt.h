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
	void EquipPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, AlchemyItem* a_poison, bool a_preventUnequip, bool a_equipSound);
	void EquipEnchantedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, bool a_preventUnequip, bool a_equipSound);
	void EquipPoisonedAndEnchantedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, AlchemyItem* a_poison, EnchantmentItem* a_enchantment, bool a_preventUnequip, bool a_equipSound);
	void EquipItemEx(Actor* a_actor, TESForm* a_item, SInt32 a_slotID, IActorEquipItem* a_iActorEquipItem, bool a_preventUnequip, bool a_equipSound);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}