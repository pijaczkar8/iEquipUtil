#pragma once

#include "GameEvents.h"  // EventDispatcher

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
}
