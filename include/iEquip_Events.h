#pragma once

#include "GameEvents.h"  // BSTEventSink, EventResult, EventDispatcher
#include "PapyrusEvents.h"  // SKSEModCallbackEvent, RegistrationSetHolder, NullParameters
#include "RE_GameEvents.h"  // RE::TESEquipEvent


namespace iEquip_Events
{
	class EquipEventHandler : public BSTEventSink<RE::TESEquipEvent>
	{
	public:
		virtual ~EquipEventHandler();
		virtual EventResult ReceiveEvent(RE::TESEquipEvent* a_event, EventDispatcher<RE::TESEquipEvent>* a_dispatcher) override;
	};


	extern RegistrationSetHolder<NullParameters> g_boundWeaponEquippedCallbackRegs;
	extern RegistrationSetHolder<NullParameters> g_boundWeaponUnequippedCallbackRegs;
	extern EquipEventHandler g_equipEventHandler;
}