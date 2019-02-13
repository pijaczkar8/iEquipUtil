#pragma once

#include "GameEvents.h"  // EventDispatcher, EventResult
#include "GameObjects.h"  // TESObjectWEAP
#include "GameReferences.h"  // Actor


namespace RE
{
	struct TESEquipEvent
	{
	public:
		TESObjectREFR*	akSource;		// 00
		UInt32			formID;			// 08
		UInt32			unk0C;			// 0C - formID of a ref handle
		UInt16			unk10;			// 10 - Related to ExtraUniqueID::uniqueID (0x14)
		bool			isEquipping;	// 12
	};

#if _WIN64
	class EventDispatcherList
	{
	public:
		EventDispatcher<void>			unk000;				//	000
		EventDispatcher<void>			unk058;				//  058 - sink offset 010
		EventDispatcher<void>			unk0B0;				//  0B0 - sink offset 018
		EventDispatcher<void>			unk108;				//  108 - sink offset 020
		EventDispatcher<void>			unk160;				//  160 - sink offset 028
		EventDispatcher<void>			unk1B8;				//  1B8 - sink offset 030
		EventDispatcher<void>			unk210;				//  210 - sink offset 038
		EventDispatcher<void>			unk2C0;				//  2C0 - sink offset 040
		EventDispatcher<void>			unk318;				//  318 - sink offset 048
		EventDispatcher<void>			unk370;				//  370 - sink offset 050
		EventDispatcher<void>			unk3C8;				//  3C8 - sink offset 058
		EventDispatcher<void>			unk420;				//  420 - sink offset 068
		EventDispatcher<void>			unk478;				//  478 - sink offset 070
		EventDispatcher<TESEquipEvent>	equipDispatcher;	//  4D0 - sink offset 078
	};

#else

	template <>
	class BSTEventSink <TESEquipEvent>
	{
	public:
		virtual ~BSTEventSink() {}
		virtual	EventResult ReceiveEvent(TESEquipEvent* a_event, EventDispatcher<TESEquipEvent>* a_dispatcher) = 0;
	};


	extern EventDispatcher<TESEquipEvent>* g_equipEventDispatcher;

#endif
}
