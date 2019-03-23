#pragma once

#include "GameEvents.h"  // BSTEventSink, EventResult, EventDispatcher
#include "PapyrusEvents.h"  // SKSEModCallbackEvent, RegistrationSetHolder, NullParameters

#include "RE/TESEquipEvent.h"  // RE::TESEquipEvent


namespace Events
{
	class EquipEventHandler : public BSTEventSink<RE::TESEquipEvent>
	{
	protected:
		EquipEventHandler();
		virtual ~EquipEventHandler();

	public:
		virtual EventResult ReceiveEvent(RE::TESEquipEvent* a_event, EventDispatcher<RE::TESEquipEvent>* a_dispatcher) override;

		static EquipEventHandler*	GetSingleton();
		static void					Free();

	private:
		EquipEventHandler(const EquipEventHandler&) = delete;
		EquipEventHandler(EquipEventHandler&&) = delete;

		EquipEventHandler&	operator=(const EquipEventHandler&) = delete;
		EquipEventHandler&	operator=(EquipEventHandler&&) = delete;


		inline static EquipEventHandler* _singleton = 0;
	};


	extern RegistrationSetHolder<NullParameters> g_boundWeaponEquippedCallbackRegs;
	extern RegistrationSetHolder<NullParameters> g_boundWeaponUnequippedCallbackRegs;
}
