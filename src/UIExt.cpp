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


TESForm* GetFormAtInventoryIndex(StaticFunctionTag* a_base, UInt32 a_index)
{
	MenuManager* mm = MenuManager::GetSingleton();
	UIStringHolder* uiStrHolder = UIStringHolder::GetSingleton();
	RE::InventoryMenu* invMenu = static_cast<RE::InventoryMenu*>(mm->GetMenu(&uiStrHolder->inventoryMenu));
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


namespace UIExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, TESForm*, UInt32>("GetFormAtInventoryIndex", "iEquip_UIExt", GetFormAtInventoryIndex, a_registry));

		return true;
	}
}
