#include "UIExt.h"

#include "GameBSExtraData.h"  // BaseExtraList, ExtendDataList
#include "GameExtraData.h"  // ExtraCount, ExtraHealth
#include "GameObjects.h"  // TESObjectREFR
#include "GameTypes.h"  // tArray
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag, NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry
#include "GameMenus.h"  // IMenu, MenuManager, UIStringHolder

#include "RE/BSTList.h"  // BSSimpleList
#include "RE/InventoryMenu.h"  // InventoryMenu

#if _WIN64
#include "Relocation.h"  // RelocAddr, RelocPtr
#include "ScaleformValue.h"  // GFxValue
#else
#include "ScaleformCallbacks.h"  // GFxValue
#endif


namespace UIExt
{
	TESForm* GetFormAtInventoryIndex(StaticFunctionTag*, UInt32 a_index)
	{
		MenuManager* mm = MenuManager::GetSingleton();
		UIStringHolder* uiStrHolder = UIStringHolder::GetSingleton();
		RE::InventoryMenu* invMenu = static_cast<RE::InventoryMenu*>(mm->GetMenu(&uiStrHolder->inventoryMenu));
		if (!invMenu) {
			_WARNING("[WARNING] Inventory menu is not open!");
			return 0;
		}

		auto& items = invMenu->inventoryData->items;
		if (a_index >= items.count) {
			_WARNING("[WARNING] Index is out of range!");
			return 0;
		}

		auto item = items[a_index];
		return item && item->data.objDesc ? item->data.objDesc->type : 0;
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, TESForm*, UInt32>("GetFormAtInventoryIndex", "iEquip_UIExt", GetFormAtInventoryIndex, a_registry));

		return true;
	}
}
