#pragma once

#include "GameEvents.h"  // BSTEventSink, EventResult, EventDispatcher
#include "PapyrusEvents.h"  // SKSEModCallbackEvent, RegistrationSetHolder, NullParameters

#include "RE/EventDispatcherList.h"  // BSTEventSink
#include "RE/TESEquipEvent.h"  // RE::TESEquipEvent


namespace Events
{
	class EquipEventHandler : public BSTEventSink<RE::TESEquipEvent>
	{
	protected:
		EquipEventHandler() = default;
		virtual ~EquipEventHandler() = default;

	public:
		virtual EventResult ReceiveEvent(RE::TESEquipEvent* a_event, EventDispatcher<RE::TESEquipEvent>* a_dispatcher) override;

		static EquipEventHandler* GetSingleton();

	private:
		EquipEventHandler(const EquipEventHandler&) = delete;
		EquipEventHandler(EquipEventHandler&&) = delete;

		EquipEventHandler&	operator=(const EquipEventHandler&) = delete;
		EquipEventHandler&	operator=(EquipEventHandler&&) = delete;
	};
}
