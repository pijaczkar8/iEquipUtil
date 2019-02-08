#pragma once

#include "skse64/GameEvents.h"  // EventDispatcher

class TESObjectREFR;
class InventoryEntryData;


namespace RE
{
	struct Inventory
	{
	public:
		struct Event
		{
		public:
			TESObjectREFR*		objRefr;	// 00
			InventoryEntryData* entryData;	// 08
			SInt32				newCount;	// 10
			SInt32				prevCount;	// 14
		};


		static EventDispatcher<Inventory::Event>* GetEventSource();
	};
}
