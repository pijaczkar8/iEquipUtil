#pragma once

#include "GameEvents.h"  // EventDispatcher, EventResult
#include "GameObjects.h"  // TESObjectWEAP
#include "GameReferences.h"  // Actor
#include "ITypes.h"  // UInt32


namespace RE
{
#if _WIN64
	class TESEquipEvent
	{
	public:
		Actor*	akSource;	// 00
		UInt32	formID;		// 08
		UInt32	pad0C;		// 0C
		UInt32	unk10;		// 10 - idk what this is
		UInt32	pad14;		// 14


		TESObjectWEAP* checkIfBoundWeapEquipEvent();
		bool isUnequipWeaponArmorEvent();
	};
	STATIC_ASSERT(offsetof(TESEquipEvent, akSource) == 0x00);
	STATIC_ASSERT(offsetof(TESEquipEvent, formID) == 0x08);
	STATIC_ASSERT(offsetof(TESEquipEvent, unk10) == 0x10);


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
	class TESEquipEvent
	{
	public:
		Actor*	akSource;	// 00
		UInt32	formID;		// 04
		UInt32	pad08;		// 08
		UInt32	unk0C;		// 0C - 0x12000000 when unequipping weapons/armor, seems to be a function ptr


		TESObjectWEAP* checkIfBoundWeapEquipEvent();
		bool isUnequipWeaponArmorEvent();
	};
	STATIC_ASSERT(offsetof(TESEquipEvent, akSource) == 0x00);
	STATIC_ASSERT(offsetof(TESEquipEvent, formID) == 0x04);
	STATIC_ASSERT(offsetof(TESEquipEvent, unk0C) == 0x0C);


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
