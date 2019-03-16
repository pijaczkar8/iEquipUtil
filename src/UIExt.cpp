#include "UIExt.h"

#include "GameObjects.h"  // TESObjectREFR
#include "GameTypes.h"  // tArray
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry
#include "GameMenus.h"  // IMenu, MenuManager, UIStringHolder

#if _WIN64
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
		UInt32				refHandle;	// 10
	};
#if _WIN64
	STATIC_ASSERT(offsetof(StandardItemData, refHandle) == 0x10);
#else
	STATIC_ASSERT(offsetof(StandardItemData, refHandle) == 0x08);
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


TESObjectREFR* GetObjectReferenceAtInventoryIndex(StaticFunctionTag* a_base, UInt32 a_index)
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

#if _WIN64
	NiPointer<TESObjectREFR> refr = 0;
	auto& item = items[a_index];
	if (item && item->data.refHandle != *g_invalidRefHandle) {
		LookupREFRByHandle(item->data.refHandle, refr);
	}
	return refr.get();
#else
	TESObjectREFR* refr = 0;
	auto& item = items[a_index];
	if (item && item->data.refHandle != *g_invalidRefHandle) {
		LookupREFRByHandle(&item->data.refHandle, &refr);
		if (refr) {
			refr->handleRefObject.DecRef();
		}
	}
	return refr;
#endif
}


namespace UIExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, TESObjectREFR*, UInt32>("GetObjectReferenceAtInventoryIndex", "iEquip_UIExt", GetObjectReferenceAtInventoryIndex, a_registry));

		return true;
	}
}
