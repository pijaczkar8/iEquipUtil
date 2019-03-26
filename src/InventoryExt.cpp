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

#include "RefHandleManager.h"  // RefHandleManager

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
		if (!a_item) {
			_ERROR("[ERROR] a_item is a NONE form!\n");
			return false;
		}

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


BSFixedString GetLongName(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle)
{
	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return "";
	}

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
	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return "";
	}

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
	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return 0;
	}

	auto xPoison = static_cast<RE::ExtraPoison*>(entryData.extraList->GetByType(kExtraData_TextDisplayData));
	return xPoison ? xPoison->count : 0;
}


void SetPoisonCount(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle, UInt32 a_newCount)
{
	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return;
	}

	auto xPoison = static_cast<RE::ExtraPoison*>(entryData.extraList->GetByType(kExtraData_TextDisplayData));
	if (xPoison) {
		xPoison->count = a_newCount;
	}
}


EnchantmentItem* GetEnchantment(StaticFunctionTag* a_base, TESForm* a_item, UInt32 a_refHandle)
{
	EntryData entryData;
	bool result = LookupEntry(a_item, a_refHandle, entryData);
	if (!result) {
		return 0;
	}

	TESEnchantableForm* enchForm = DYNAMIC_CAST(a_item, TESForm, TESEnchantableForm);
	if (enchForm) {
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
			new NativeFunction2<StaticFunctionTag, BSFixedString, TESForm*, UInt32>("GetLongName", "iEquip_ObjectReferenceExt", GetLongName, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, BSFixedString, TESForm*, UInt32>("GetShortName", "iEquip_ObjectReferenceExt", GetShortName, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, SInt32, TESForm*, UInt32>("GetPoisonCount", "iEquip_ObjectReferenceExt", GetPoisonCount, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction3<StaticFunctionTag, void, TESForm*, UInt32, UInt32>("SetPoisonCount", "iEquip_ObjectReferenceExt", SetPoisonCount, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, EnchantmentItem*, TESForm*, UInt32>("GetEnchantment", "iEquip_ObjectReferenceExt", GetEnchantment, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction6<StaticFunctionTag, void, TESForm*, UInt32, Actor*, SInt32, bool, bool>("EquipItem", "iEquip_ObjectReferenceExt", EquipItem, a_registry));

		return true;
	}
}
