#pragma once

#include "GameTypes.h"  // BSFixedString

class Actor;
class EnchantmentItem;
class TESForm;
class VMClassRegistry;
struct StaticFunctionTag;


void				RegisterForRefHandleActiveEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);
void				UnregisterForRefHandleActiveEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);
void				RegisterForOnRefHandleInvalidatedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);
void				UnregisterForOnRefHandleInvalidatedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm);
void				ParseInventory(StaticFunctionTag* a_base);
BSFixedString		GetLongName(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle);
BSFixedString		GetShortName(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle);
SInt32				GetPoisonCount(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle);
void				SetPoisonCount(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle, UInt32 a_newCount);
EnchantmentItem*	GetEnchantment(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle);
void				EquipItem(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle, Actor* a_actor, SInt32 a_equipSlot, bool a_preventUnequip, bool a_equipSound);


namespace InventoryExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
