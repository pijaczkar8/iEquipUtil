#include "UIExt.h"

#include "GameBSExtraData.h"  // BaseExtraList, ExtendDataList
#include "GameExtraData.h"  // ExtraCount, ExtraHealth
#include "GameObjects.h"  // TESObjectREFR
#include "GameTypes.h"  // tArray
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry
#include "GameMenus.h"  // IMenu, MenuManager, UIStringHolder

#if _WIN64
#include "Relocation.h"  // RelocAddr, RelocPtr
#include "ScaleformValue.h"  // GFxValue
#else
#include "ScaleformCallbacks.h"  // GFxValue
#endif


namespace
{
	class StandardItemData
	{
	public:
		virtual ~StandardItemData();

		virtual const char*	GetName();
		virtual UInt32		GetCount();
		virtual UInt32		GetEquipState();
		virtual UInt32		GetFilterFlag();
		virtual UInt32		GetFavorite();
		virtual bool		GetEnabled();


		// members
		InventoryEntryData*	objDesc;	// 08
	};
#if _WIN64
	STATIC_ASSERT(offsetof(StandardItemData, objDesc) == 0x8);
#else
	STATIC_ASSERT(offsetof(StandardItemData, objDesc) == 0x04);
#endif


	class InventoryMenu : public IMenu
	{
	public:
		struct InventoryData
		{
			struct Item
			{
				StandardItemData data;	// 00
			};


			// members
			void*			unk00;				// 00
#ifndef _WIN64
			void*			unk;
#endif
			GFxValue		categoryListRoot;	// 08 - kDisplayObject
			GFxValue		unk20;				// 20 - kArray
			tArray<Item*>	items;				// 38
		};
#if _WIN64
		STATIC_ASSERT(offsetof(InventoryData, items) == 0x38);
#else
		STATIC_ASSERT(offsetof(InventoryData, items) == 0x28);
#endif


		virtual ~InventoryMenu();	// 00


		// members
#ifndef _WIN64
		UInt32			pad;
#endif
		GFxValue		root;			// 30 - kDisplayObject
		InventoryData*	inventoryData;	// 48
	};
#if _WIN64
	STATIC_ASSERT(offsetof(InventoryMenu, inventoryData) == 0x48);
#else
	STATIC_ASSERT(offsetof(InventoryMenu, inventoryData) == 0x30);
#endif
}


TESForm* GetFormAtInventoryIndex(StaticFunctionTag* a_base, UInt32 a_index)
{
	MenuManager* mm = MenuManager::GetSingleton();
	UIStringHolder* uiStrHolder = UIStringHolder::GetSingleton();
	InventoryMenu* invMenu = static_cast<InventoryMenu*>(mm->GetMenu(&uiStrHolder->inventoryMenu));
	if (!invMenu) {
		_ERROR("[ERROR] Inventory menu is not open!\n");
		return 0;
	}

	auto& items = invMenu->inventoryData->items;
	if (a_index >= items.count) {
		_ERROR("[ERROR] Index is out of range!\n");
		return 0;
	}

	auto item = items[a_index];
	return item && item->data.objDesc ? item->data.objDesc->type : 0;
}


BSFixedString GetTemperStringAtInventoryIndex(StaticFunctionTag* a_base, UInt32 a_index, SInt32 a_count)
{
	MenuManager* mm = MenuManager::GetSingleton();
	UIStringHolder* uiStrHolder = UIStringHolder::GetSingleton();
	InventoryMenu* invMenu = static_cast<InventoryMenu*>(mm->GetMenu(&uiStrHolder->inventoryMenu));
	if (!invMenu) {
		_ERROR("[ERROR] Inventory menu is not open!\n");
		return "";
	}

	auto& items = invMenu->inventoryData->items;
	if (a_index >= items.count) {
		_ERROR("[ERROR] Index is out of range!\n");
		return "";
	}

	auto item = items[a_index];
	InventoryEntryData* entryData = item ? item->data.objDesc : 0;
	if (!entryData) {
		_ERROR("[ERROR] Item does not have entry data!\n");
		return "";
	}

	using func_t = const char*(float a_temperFactor, bool a_isWeapon);
#if _WIN64
	// E8 ? ? ? ? F3 0F 11 73 2C
	constexpr std::uintptr_t FUNC_CALL = 0x0013CC20 + 0x59;	// 1_5_73
	RelocPtr<SInt32> callAddr(FUNC_CALL + 0x1);
	RelocAddr<std::uintptr_t> absAddr(FUNC_CALL + 0x5);
	func_t* func = reinterpret_cast<func_t*>(absAddr.GetUIntPtr() + *callAddr);
#else
	constexpr std::uintptr_t FUNC_CALL = 0x004278A0 + 0xB5;
	SInt32 callAddr = *reinterpret_cast<SInt32*>(FUNC_CALL + 0x1);
	std::uintptr_t absAddr = FUNC_CALL + 0x5;
	func_t* func = reinterpret_cast<func_t*>(absAddr + callAddr);
#endif

	BaseExtraList* xList = 0;
	ExtendDataList* extendDataList = entryData->extendDataList;
	if (extendDataList) {
		xList = extendDataList->GetNthItem(0);
		ExtraCount* xCount = static_cast<ExtraCount*>(xList->GetByType(kExtraData_Count));
		if (!xCount || a_count > xCount->count) {
			xList = 0;
		}
	}

	if (xList) {
		ExtraHealth* xHealth = static_cast<ExtraHealth*>(xList->GetByType(kExtraData_Health));
		if (xHealth) {
			bool isWeapon = entryData->type && entryData->type->formType == kFormType_Weapon;
			return func(xHealth->health, isWeapon);
		}
	}
	return "";
}


namespace UIExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, TESForm*, UInt32>("GetFormAtInventoryIndex", "iEquip_UIExt", GetFormAtInventoryIndex, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, BSFixedString, UInt32, SInt32>("GetTemperStringAtInventoryIndex", "iEquip_UIExt", GetTemperStringAtInventoryIndex, a_registry));

		return true;
	}
}
