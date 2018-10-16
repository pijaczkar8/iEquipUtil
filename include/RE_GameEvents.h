#pragma once

#include "GameEvents.h"  // EventDispatcher
#include "GameForms.h"  // TESForm
#include "GameReferences.h"  // Actor
#include "ITypes.h"  // UInt8, UInt32, UInt64


namespace RE
{
#if _WIN64
	class TESEquipEvent
	{
	public:
		Actor* akSource;  // 00
		UInt32 formID;  // 08
		UInt32 pad0C;  // 0C


		bool checkIfBoundWeapEquipped();

		// Do NOT call this unless the event is thrown for a weapon!
		UInt8 getWeaponType();

	private:
		// This section is incomplete
		UInt64 unk10;  // 10 - 0xFF000000 when unequipping weapons/armor
		UInt64 unk18;  // 18
		UInt64 unk20;  // 20
		TESForm* unk28;  // 28 - Valid when equipping/unequipping weapons/armor
		UInt64 unk30;  // 30
		UInt64 unk38;  // 38
		UInt64 unk40;  // 40
		UInt64 unk48;  // 48
		UInt64 unk50;  // 50
		UInt64 unk58;  // 58
		UInt64 unk60;  // 60 - 0x1 when equipping weapons/armor
	};
#else
	class TESEquipEvent
	{
	public:
		Actor* akSource;  // 00
		UInt32 formID;  // 04

		bool checkIfBoundWeapEquipped();

		// Do NOT call this unless the event is thrown for a weapon!
		UInt8 getWeaponType();

	private:
		// This section is incomplete
		UInt64 unk10;  // 10 - 0xFF000000 when unequipping weapons/armor
		UInt64 unk18;  // 18
		UInt64 unk20;  // 20
		TESForm* unk28;  // 28 - Valid when equipping/unequipping weapons/armor
		UInt64 unk30;  // 30
		UInt64 unk38;  // 38
		UInt64 unk40;  // 40
		UInt64 unk48;  // 48
		UInt64 unk50;  // 50
		UInt64 unk58;  // 58
		UInt64 unk60;  // 60 - 0x1 when equipping weapons/armor
	};
#endif


	// These are SSE offsets, but the event dispatcher is in the same position
	class EventDispatcherList
	{
	public:
		EventDispatcher<void>			unk00;				//	00
		EventDispatcher<void>			unk58;				//  58  - sink offset 010
		EventDispatcher<void>			unkB0;				//  B0  - sink offset 018
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
}