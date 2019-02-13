#pragma once

#include "GameEvents.h"  // EventDispatcher, BSTEventSink

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


#ifndef _WIN64
	template <>
	class BSTEventSink<Inventory::Event>
	{
	public:
		virtual ~BSTEventSink() {}
		virtual	EventResult ReceiveEvent(Inventory::Event* a_event, EventDispatcher<Inventory::Event>* a_dispatcher) = 0;
	};
#endif
}
