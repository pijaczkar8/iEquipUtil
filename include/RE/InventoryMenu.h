#pragma once

#include "GameMenus.h"  // IMenu


namespace RE
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
