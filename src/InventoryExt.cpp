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
#include "RE/InventoryMenu.h"  // InventoryMenu


namespace
{
	using EntryData = RefHandleManager::EntryData;


	enum : UInt32
	{
		kSlotID_Default = 0,
		kSlotID_Right = 1,
		kSlotID_Left = 2
	};


	enum : UInt32
	{
		kXSlotID_Head = 0,
		kXSlotID_Chest = 1,
		kXSlotID_Boots = 2,
		kXSlotID_Gloves = 3,
		kXSlotID_RightHand = 4,
		kXSlotID_LeftHand = 5
	};


	enum : UInt32
	{
		kFirstPersonFlag_None = 0,
		kFirstPersonFlag_Head = 1 << 0,
		kFirstPersonFlag_Hair = 1 << 1,
		kFirstPersonFlag_Body = 1 << 2,
		kFirstPersonFlag_Hands = 1 << 3,
		kFirstPersonFlag_Forearms = 1 << 4,
		kFirstPersonFlag_Amulet = 1 << 5,
		kFirstPersonFlag_Ring = 1 << 6,
		kFirstPersonFlag_Feet = 1 << 7,
		kFirstPersonFlag_Calves = 1 << 8,
		kFirstPersonFlag_Shield = 1 << 9,
		kFirstPersonFlag_Tail = 1 << 10,
		kFirstPersonFlag_LongHair = 1 << 11,
		kFirstPersonFlag_Circlet = 1 << 12,
		kFirstPersonFlag_Ears = 1 << 13,
		kFirstPersonFlag_DecapitateHead = 1 << 20,
		kFirstPersonFlag_Decapitate = 1 << 21,
		kFirstPersonFlag_FX01 = (UInt32)(1 << 31)
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


	bool GetSlotByID(UInt32 a_equipSlot, TESForm* a_item, bool a_worn, bool a_wornLeft, BGSEquipSlot*& a_slotOut)
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


	bool GetWornObjectFilters(UInt32 a_equipSlot, UInt32& a_xDataType, UInt32& a_formType, UInt32& a_firstPersonFlag)
	{
		switch (a_equipSlot) {
		case kXSlotID_Head:
		case kXSlotID_Chest:
		case kXSlotID_Boots:
		case kXSlotID_Gloves:
		case kXSlotID_RightHand:
			a_xDataType = kExtraData_Worn;
			break;
		case kXSlotID_LeftHand:
			a_xDataType = kExtraData_WornLeft;
			break;
		default:
			_ERROR("[ERROR] Invalid slot ID!\n");
			return false;
		}

		switch (a_equipSlot) {
		case kXSlotID_Head:
		case kXSlotID_Chest:
		case kXSlotID_Boots:
		case kXSlotID_Gloves:
			a_formType = kFormType_Armor;
			break;
		case kXSlotID_RightHand:
		case kXSlotID_LeftHand:
			a_formType = kFormType_Weapon;
			break;
		default:
			_ERROR("[ERROR] Invalid slot ID!\n");
			return false;
		}

		switch (a_equipSlot) {
		case kXSlotID_Head:
			a_firstPersonFlag = kFirstPersonFlag_Circlet;
			break;
		case kXSlotID_Chest:
			a_firstPersonFlag = kFirstPersonFlag_Body;
			break;
		case kXSlotID_Boots:
			a_firstPersonFlag = kFirstPersonFlag_Feet;
			break;
		case kXSlotID_Gloves:
			a_firstPersonFlag = kFirstPersonFlag_Hands;
			break;
		case kXSlotID_RightHand:
		case kXSlotID_LeftHand:
			a_firstPersonFlag = kFirstPersonFlag_None;
			break;
		default:
			_ERROR("[ERROR] Invalid slot ID!\n");
			return false;
		}

		return true;
	}
}


void RegisterForRefHandleActiveEvent(StaticFunctionTag*, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleActiveRegSet::GetSingleton()->Register<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Registered (0x%08X) for OnRefHandleActive", a_thisForm->formID);
	}
}


void UnregisterForRefHandleActiveEvent(StaticFunctionTag*, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleActiveRegSet::GetSingleton()->Unregister<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Unregistered (0x%08X) for OnRefHandleActive", a_thisForm->formID);
	}
}


void RegisterForOnRefHandleInvalidatedEvent(StaticFunctionTag*, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleInvalidatedRegSet::GetSingleton()->Register<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Registered (0x%08X) for OnRefHandleInvalidated", a_thisForm->formID);
	}
}


void UnregisterForOnRefHandleInvalidatedEvent(StaticFunctionTag*, TESForm* a_thisForm)
{
	if (!a_thisForm) {
		_ERROR("[ERROR] a_thisForm is a NONE form!\n");
		return;
	} else {
		OnRefHandleInvalidatedRegSet::GetSingleton()->Unregister<TESForm>(a_thisForm->formType, a_thisForm);
		_DMESSAGE("[DEBUG] Unregistered (0x%08X) for OnRefHandleInvalidated", a_thisForm->formID);
	}
}


void ParseInventory(StaticFunctionTag*)
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


UInt32 GetRefHandleAtInvIndex(StaticFunctionTag*, UInt32 a_index)
{
	MenuManager* mm = MenuManager::GetSingleton();
	UIStringHolder* uiStrHolder = UIStringHolder::GetSingleton();
	RE::InventoryMenu* invMenu = static_cast<RE::InventoryMenu*>(mm->GetMenu(&uiStrHolder->inventoryMenu));
	if (!invMenu) {
		_ERROR("[ERROR] Inventory menu is not open!\n");
		return RefHandleManager::kInvalidRefHandle;
	}

	auto& items = invMenu->inventoryData->items;
	if (a_index >= items.count) {
		_ERROR("[ERROR] Index is out of range!\n");
		return RefHandleManager::kInvalidRefHandle;
	}

	auto item = items[a_index];
	if (!item) {
		_ERROR("[ERROR] Failed to find item at index!\n");
		return RefHandleManager::kInvalidRefHandle;
	}

	if (!item->data.objDesc->extendDataList) {
		_ERROR("[ERROR] Item did not have extra data!\n");
		return RefHandleManager::kInvalidRefHandle;
	}

	ExtraUniqueID* xID = 0;
	ForEachExtraList(item->data.objDesc, [&](BaseExtraList* a_extraList) -> bool
	{
		xID = static_cast<ExtraUniqueID*>(a_extraList->GetByType(kExtraData_UniqueID));
		return xID == 0;
	});
	if (!xID) {
		_ERROR("[ERROR] Could not find unique ID for item!\n");
		return RefHandleManager::kInvalidRefHandle;
	}

	auto manager = RefHandleManager::GetSingleton();
	auto handle = manager->LookupHandle(xID->uniqueId);
	if (handle == RefHandleManager::kInvalidRefHandle) {
		_ERROR("[ERROR] Could not find handle for unique ID!\n");
	}
	return handle;
}


UInt32 GetRefHandleFromWornObject(StaticFunctionTag*, UInt32 a_equipSlot)
{
	UInt32 handle = RefHandleManager::kInvalidRefHandle;
	UInt32 xDataType;
	UInt32 formType;
	UInt32 firstPersonFlag;
	if (!GetWornObjectFilters(a_equipSlot, xDataType, formType, firstPersonFlag)) {
		return handle;
	}

	ForEachInvEntry([&](InventoryEntryData* a_entryData) -> bool
	{
		bool found = false;
		if (a_entryData->type->formType == formType && a_entryData->extendDataList) {
			switch (a_equipSlot) {
			case kXSlotID_Head:
			case kXSlotID_Chest:
			case kXSlotID_Boots:
			case kXSlotID_Gloves:
				{
					auto armor = static_cast<TESObjectARMO*>(a_entryData->type);
					if ((armor->bipedObject.data.parts & firstPersonFlag) == 0) {
						return true;
					}
				}
				break;
			default:
				break;
			}

			ForEachExtraList(a_entryData, [&](BaseExtraList* a_extraList) -> bool
			{
				if (a_extraList->HasType(xDataType)) {
					auto xID = static_cast<ExtraUniqueID*>(a_extraList->GetByType(kExtraData_UniqueID));
					if (xID) {
						auto manager = RefHandleManager::GetSingleton();
						handle = manager->LookupHandle(xID->uniqueId);
						if (handle != RefHandleManager::kInvalidRefHandle) {
							found = true;
							return false;
						}
					}
				}
				return true;
			});
		}
		return !found;
	});

	if (handle == RefHandleManager::kInvalidRefHandle) {
		_ERROR("[ERROR] Failed to find ref handle!\n");
	}
	return handle;
}


BSFixedString GetLongName(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle)
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


BSFixedString GetShortName(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle)
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


SInt32 GetPoisonCount(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle)
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


void SetPoisonCount(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle, UInt32 a_newCount)
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


EnchantmentItem* GetEnchantment(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle)
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


void EquipItem(StaticFunctionTag*, TESForm* a_item, UInt32 a_refHandle, Actor* a_actor, UInt32 a_equipSlot, bool a_preventUnequip, bool a_equipSound)
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
			new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("GetRefHandleAtInvIndex", "iEquip_InventoryExt", GetRefHandleAtInvIndex, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, UInt32, UInt32>("GetRefHandleFromWornObject", "iEquip_InventoryExt", GetRefHandleFromWornObject, a_registry));

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
			new NativeFunction6<StaticFunctionTag, void, TESForm*, UInt32, Actor*, UInt32, bool, bool>("EquipItem", "iEquip_InventoryExt", EquipItem, a_registry));

		return true;
	}
}
