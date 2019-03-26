#pragma once

class Actor;
class AlchemyItem;
class EnchantmentItem;
class IActorEquipItem;
class TESAmmo;
class TESForm;
class TESRace;
class VMClassRegistry;
struct StaticFunctionTag;


TESAmmo*	GetEquippedAmmo(StaticFunctionTag* a_base, Actor* a_actor);
void		EquipEnchantedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, bool a_preventUnequip = false, bool a_equipSound = true);
void		EquipPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, AlchemyItem* a_poison, UInt32 a_count = 1, bool a_preventUnequip = false, bool a_equipSound = true);
void		EquipEnchantedAndPoisonedItemEx(StaticFunctionTag* a_base, Actor* a_actor, TESForm* a_item, SInt32 a_slotID, EnchantmentItem* a_enchantment, AlchemyItem* a_poison, UInt32 a_count = 1, bool a_preventUnequip = false, bool a_equipSound = true);
float		GetAVDamage(StaticFunctionTag* a_base, Actor* a_actor, UInt32 a_actorValue);
TESRace*	GetBaseRace(StaticFunctionTag* a_base, Actor* a_actor);


namespace ActorExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
