#include "InventoryExt.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameData.h"  // EquipManager
#include "GameExtraData.h"  // ExtraCount
#include "GameFormComponents.h"  // TESFullName
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "GameTypes.h"  // BSFixedString
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <string>  // string

#include "InventoryUtil.h"  // ForEachInvEntry, ForEachExtraList
#include "RefHandleManager.h"  // RefHandleManager
#include "Registration.h"  // OnRefHandleActiveRegSet, OnRefHandleInvalidatedRegSet

#include "RE/ExtraPoison.h"  // ExtraPoison
#include "RE/ExtraTextDisplayData.h"  // ExtraTextDisplayData


namespace
{
	using EntryData = RefHandleManager::EntryData;


	enum
	{
		kSlotID_Default = 0,
		kSlotID_Right = 1,
		kSlotID_Left = 2
	};


	bool LookupEntry(TESForm* a_item, UInt32 a_refHandle, EntryData& a_entryDataOut)
	{
		RefHandleManager* refHandleManager = RefHandleManager::GetSingleton();
		auto result = refHandleManager->LookupEntry(a_item, a_refHandle);
		if (!result.invEntryData || !result.extraList) {
			_ERROR("[ERROR] Failed to find item!\n");
			return false;
		} else {
			a_entryDataOut = result;
			return true;
		}
	}


	bool GetSlotByID(SInt32 a_equipSlot, TESForm* a_item, bool a_worn, bool a_wornLeft, BGSEquipSlot*& a_slotOut)
	{
		switch (a_equipSlot) {
		case kSlotID_Default:
			if (a_item->IsWeapon()) {
				_ERROR("[ERROR] a_equipSlot is an invalid slot for a_item!\n");
				return false;
			} else {
				if (a_worn) {
					_ERROR("[ERROR] a_item is already worn!\n");
					return false;
				} else {
					a_slotOut = 0;
				}
			}
			break;
		case kSlotID_Right:
			if (!a_item->IsWeapon()) {
				_ERROR("[ERROR] a_equipSlot is an invalid slot for a_item!\n");
				return false;
			} else {
				if (a_worn) {
					_ERROR("[ERROR] a_item is already worn!\n");
					return false;
				} else {
					a_slotOut = GetRightHandSlot();
				}
			}
			break;
		case kSlotID_Left:
			if (!a_item->IsWeapon()) {
				_ERROR("[ERROR] a_equipSlot is an invalid slot for a_item!\n");
				return false;
			} else {
				if (a_wornLeft) {
					_ERROR("[ERROR] a_item is already worn!\n");
					return false;
				} else {
					a_slotOut = GetLeftHandSlot();
				}
			}
			break;
		default:
			_ERROR("[ERROR] a_equipSlot is not a slot (%i)!\n", a_equipSlot);
			return false;
		}
		return true;
	}
}


void RegisterForRefHandleActiveEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleActiveRegSet::GetSingleton()->Register<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Registered (0x%08X) for OnRefHandleActive", a_thisForm->formID);
	}
}


void UnregisterForRefHandleActiveEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleActiveRegSet::GetSingleton()->Unregister<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Unregistered (0x%08X) for OnRefHandleActive", a_thisForm->formID);
	}
}


void RegisterForOnRefHandleInvalidatedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleInvalidatedRegSet::GetSingleton()->Register<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Registered (0x%08X) for OnRefHandleInvalidated", a_thisForm->formID);
	}
}


void UnregisterForOnRefHandleInvalidatedEvent(StaticFunctionTag* a_base, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleInvalidatedRegSet::GetSingleton()->Unregister<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Unregistered (0x%08X) for OnRefHandleInvalidated", a_thisForm->formID);
	}
}


void ParseInventory(StaticFunctionTag* a_base)
{
	auto manager = RefHandleManager::GetSingleton();
	auto regs = OnRefHandleActiveRegSet::GetSingleton();
	ForEachInvEntry([&](InventoryEntryData* a_entryData) -> bool
	{
		if (manager->IsTrackedType(a_entryData->type)) {
			SInt32 rawCount = a_entryData->countDelta;
			if (a_entryData->extendDataList) {
				ForEachExtraList(a_entryData, [&](BaseExtraList* a_extraList) -> bool
				{
					auto xCount = static_cast<ExtraCount*>(a_extraList->GetByType(kExtraData_Count));
					SInt32 count = xCount ? xCount->count : 1;
					rawCount -= count;
					auto result = manager->ActivateHandle(a_entryData->type, a_extraList);
					if (result.second) {
						regs->QueueEvent(a_entryData->type, result.first, count);
					}
					return true;
				});
			}
			BaseExtraList* xListOut;
			for (SInt32 i = 0; i < rawCount; ++i) {
				xListOut = 0;
				auto result = manager->ActivateHandle(a_entryData->type, xListOut);
				if (result.second) {
					regs->QueueEvent(a_entryData->type, result.first, 1);
				}
			}
		}
		return true;
	});
}


BSFixedString GetLongName(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle)
{
	if (!a_item) {
		_ERROR("[ERROR] a_item is a NONE form!\n");
		return "";
	}

	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return "";
	}

	entryData.extraList->GetDisplayName(a_item);
	auto xText = static_cast<RE::ExtraTextDisplayData*>(entryData.extraList->GetByType(kExtraData_TextDisplayData));
	if (xText) {
		return xText->name;
	} else {
		TESFullName* name = DYNAMIC_CAST(a_item, TESForm, TESFullName);
		return name ? name->GetName() : "";
	}
}


BSFixedString GetShortName(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle)
{
	if (!a_item) {
		_ERROR("[ERROR] a_item is a NONE form!\n");
		return "";
	}

	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return "";
	}

	entryData.extraList->GetDisplayName(a_item);
	auto xText = static_cast<RE::ExtraTextDisplayData*>(entryData.extraList->GetByType(kExtraData_TextDisplayData));
	if (xText) {
		std::string name(xText->name.data, xText->rawNameLen);
		return name.c_str();
	} else {
		TESFullName* name = DYNAMIC_CAST(a_item, TESForm, TESFullName);
		return name ? name->GetName() : "";
	}
}


SInt32 GetPoisonCount(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle)
{
	if (!a_item) {
		_ERROR("[ERROR] a_item is a NONE form!\n");
		return 0;
	} else if (a_item->formType != kFormType_Weapon) {
		_ERROR("[ERROR] a_item is not a weapon!\n");
		return 0;
	}

	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return 0;
	}

	auto xPoison = static_cast<RE::ExtraPoison*>(entryData.extraList->GetByType(kExtraData_Poison));
	return xPoison ? xPoison->count : 0;
}


void SetPoisonCount(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle, UInt32 a_newCount)
{
	if (!a_item) {
		_ERROR("[ERROR] a_item is a NONE form!\n");
		return;
	} else if (a_item->formType != kFormType_Weapon) {
		_ERROR("[ERROR] a_item is not a weapon!\n");
		return;
	}

	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return;
	}

	auto xPoison = static_cast<RE::ExtraPoison*>(entryData.extraList->GetByType(kExtraData_Poison));
	if (xPoison) {
		xPoison->count = a_newCount;
	}
}


EnchantmentItem* GetEnchantment(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle)
{
	if (!a_item) {
		_ERROR("[ERROR] a_item is a NONE form!\n");
		return false;
	}

	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return 0;
	}

	TESEnchantableForm* enchForm = DYNAMIC_CAST(a_item, TESForm, TESEnchantableForm);
	if (enchForm && enchForm->enchantment) {
		return enchForm->enchantment;
	} else {
		auto xEnch = static_cast<ExtraEnchantment*>(entryData.extraList->GetByType(kExtraData_Enchantment));
		return xEnch ? xEnch->enchant : 0;
	}
}


void EquipItem(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle, Actor* a_actor, SInt32 a_equipSlot, bool a_preventUnequip, bool a_equipSound)
{
	if (!a_actor) {
		_ERROR("[ERROR] a_actor is a NONE form!\n");
		return;
	}

	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return;
	}

	bool worn = false;
	bool wornLeft = false;
	SInt32 count = 1;
	if (entryData.extraList) {
		auto xCount = static_cast<ExtraCount*>(entryData.extraList->GetByType(kExtraData_Count));
		count = xCount->count;

		auto xWorn = static_cast<ExtraWorn*>(entryData.extraList->GetByType(kExtraData_Worn));
		worn = xWorn != 0;

		auto xWornLeft = static_cast<ExtraWornLeft*>(entryData.extraList->GetByType(kExtraData_WornLeft));
		wornLeft = xWornLeft != 0;
	}

	BGSEquipSlot* slot;
	if (!GetSlotByID(a_equipSlot, a_item, worn, wornLeft, slot)) {
		return;
	}

	if (worn || wornLeft) {
		if (count < 2) {
			_ERROR("[ERROR] Item count is too small to equip!\n");
			return;
		}
	} else {
		if (count < 1) {
			_ERROR("[ERROR] Item count is too small to equip!\n");
			return;
		}
	}

	SInt32 equipCount;
	BaseExtraList* extraList;
	if (a_item->IsAmmo()) {
		equipCount = count;
		extraList = 0;
	} else {
		equipCount = 1;
		extraList = entryData.extraList;
	}

	EquipManager* equipManager = EquipManager::GetSingleton();
	CALL_MEMBER_FN(equipManager, EquipItem)(a_actor, a_item, extraList, equipCount, slot, a_equipSound, a_preventUnequip, false, 0);
}


namespace InventoryExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForRefHandleActiveEvent", "iEquip_InventoryExt", RegisterForRefHandleActiveEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("UnregisterForRefHandleActiveEvent", "iEquip_InventoryExt", UnregisterForRefHandleActiveEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("RegisterForOnRefHandleInvalidatedEvent", "iEquip_InventoryExt", RegisterForOnRefHandleInvalidatedEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, void, TESForm*>("UnregisterForOnRefHandleInvalidatedEvent", "iEquip_InventoryExt", UnregisterForOnRefHandleInvalidatedEvent, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction0<StaticFunctionTag, void>("ParseInventory", "iEquip_InventoryExt", ParseInventory, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, BSFixedString, TESForm*, UInt32>("GetLongName", "iEquip_InventoryExt", GetLongName, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, BSFixedString, TESForm*, UInt32>("GetShortName", "iEquip_InventoryExt", GetShortName, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, SInt32, TESForm*, UInt32>("GetPoisonCount", "iEquip_InventoryExt", GetPoisonCount, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction3<StaticFunctionTag, void, TESForm*, UInt32, UInt32>("SetPoisonCount", "iEquip_InventoryExt", SetPoisonCount, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, EnchantmentItem*, TESForm*, UInt32>("GetEnchantment", "iEquip_InventoryExt", GetEnchantment, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction6<StaticFunctionTag, void, TESForm*, UInt32, Actor*, SInt32, bool, bool>("EquipItem", "iEquip_InventoryExt", EquipItem, a_registry));

		return true;
	}
}
