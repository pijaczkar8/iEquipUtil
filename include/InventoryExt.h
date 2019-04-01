#pragma once

#include "GameTypes.h"  // BSFixedString

class Actor;
class EnchantmentItem;
class TESForm;
class VMClassRegistry;
struct StaticFunctionTag;


void				RegisterForRefHandleActiveEvent(StaticFunctionTag*, TESForm* a_thisForm);
void				UnregisterForRefHandleActiveEvent(StaticFunctionTag*, TESForm* a_thisForm);
void				RegisterForOnRefHandleInvalidatedEvent(StaticFunctionTag*, TESForm* a_thisForm);
void				UnregisterForOnRefHandleInvalidatedEvent(StaticFunctionTag*, TESForm* a_thisForm);
void				ParseInventory(StaticFunctionTag*);
UInt32				GetRefHandleAtInvIndex(StaticFunctionTag*, UInt32 a_index);
UInt32				GetRefHandleFromWornObject(StaticFunctionTag*, UInt32 a_equipSlot);
BSFixedString		GetLongName(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle);
BSFixedString		GetShortName(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle);
SInt32				GetPoisonCount(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle);
void				SetPoisonCount(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle, UInt32 a_newCount);
EnchantmentItem*	GetEnchantment(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle);
void				EquipItem(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle, Actor* a_actor, UInt32 a_equipSlot, bool a_preventUnequip, bool a_equipSound);


namespace InventoryExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
