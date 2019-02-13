#pragma once

#include "GameEvents.h"  // EventDispatcher, BSTEventSink

class TESForm;


namespace RE
{
	struct ItemCrafted
	{
	public:
		struct Event
		{
		public:
			TESForm*	item;	// 00
			bool		unk08;	// 08 - weapon smithing/alchemy = true
			bool		unk09;	// 09 - enchanting = true
			bool		unk0A;	// 0A - weapon smithing = true/false
			UInt8		pad0B;	// 0B
			UInt32		pad0C;	// 0C
		};


		static EventDispatcher<ItemCrafted::Event>* GetEventSource();
	};


#ifndef _WIN64
	template <>
	class BSTEventSink<ItemCrafted::Event>
	{
	public:
		virtual ~BSTEventSink() {}
		virtual	EventResult ReceiveEvent(ItemCrafted::Event* a_event, EventDispatcher<ItemCrafted::Event>* a_dispatcher) = 0;
	};
#endif
}
