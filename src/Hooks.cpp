#include"Hooks.h"

#if _WIN64
#include "Relocation.h"  // RelocPtr
#endif
#include "SafeWrite.h"  // SafeWrite64
#include "GameBSExtraData.h"  // BaseExtraList
#include "GameForms.h"  // TESForm
#include "GameObjects.h"  // TESObjectREFR

#include <memory>  // shared_ptr

#include "Armor.h"  // Armor
#include "InventoryHandler.h"  // InventoryHandler
#include "SerializableFormFactory.h"  // SerializableFormFactory


namespace iEquip
{
	typedef UInt32& _RemoveItem_t(PlayerCharacter* a_this, UInt32& a_droppedItemHandle, TESForm* a_item, UInt32 a_count, UInt32 a_mode, BaseExtraList* a_extraList, TESObjectREFR* a_moveToRef, void* a_arg7, void* a_arg8);
	_RemoveItem_t* _RemoveItem;


	UInt32& Hook_RemoveItem(PlayerCharacter* a_this, UInt32& a_droppedItemHandle, TESForm* a_item, UInt32 a_count, UInt32 a_mode, BaseExtraList* a_extraList, TESObjectREFR* a_moveToRef, void* a_arg7, void* a_arg8)
	{
		if (a_item) {
			Serializable::FormFactory* formFactory = Serializable::FormFactory::GetSingleton();
			SerializableFormPtr ptr = formFactory->GetForm(a_item->formType, true);
			if (ptr) {
				ptr->Set(a_item, a_extraList);
				InventoryHandler* invHandler = InventoryHandler::GetSingleton();
				invHandler->AddForm(ptr, a_count * -1);
				bool dummy = true;
			}
		}
		return _RemoveItem(a_this, a_droppedItemHandle, a_item, a_count, a_mode, a_extraList, a_moveToRef, a_arg7, a_arg8);
	}


	void InstallRemoveItemHook()
	{
#if _WIN64
		constexpr std::uintptr_t PLAYER_CHARACTER_VTBL = 0x0167D640;	// 1_5_62
		RelocPtr<_RemoveItem_t*> vFunc(PLAYER_CHARACTER_VTBL + (0x8 * 0x56));
		_RemoveItem = *vFunc;
		SafeWrite64(vFunc.GetUIntPtr(), GetFnAddr(&Hook_RemoveItem));
#endif

		_MESSAGE("[MESSAGE] Installed remove item hook");
	}


	void InstallHooks()
	{
		InstallRemoveItemHook();
	}
}
