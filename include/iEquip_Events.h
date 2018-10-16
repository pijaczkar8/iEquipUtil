#pragma once

#include "GameEvents.h"  // BSTEventSink, EventResult, EventDispatcher
#include "PapyrusEvents.h"  // SKSEModCallbackEvent, RegistrationSetHolder, NullParameters
#include "RE_GameEvents.h"  // RE::TESEquipEvent


namespace iEquip_Events
{
	class EquipEventHandler : public BSTEventSink<RE::TESEquipEvent>
	{
	public:
		EventResult ReceiveEvent(RE::TESEquipEvent* a_event, EventDispatcher<RE::TESEquipEvent>* a_dispatcher);
	};


	extern RegistrationSetHolder<NullParameters> g_callbackRegs;
	extern EventDispatcher<SKSEModCallbackEvent>* g_callbackDispatcher;
	extern EquipEventHandler g_equipEventHandler;
}