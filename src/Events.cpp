#include "Events.h"

#include "GameAPI.h"  // g_thePlayer
#include "GameEvents.h"  // EventResult, EventDispatcher
#include "GameObjects.h"  // TESObjectWEAP

#include <limits>  // numeric_limits

#include "ActorExtLib.h"  // GetEquippedSlots
#include "Registration.h"  // OnBoundWeaponEquippedRegSet, OnBoundWeaponUnequippedRegSet

#include "RE/TESEquipEvent.h"  // TESEquipEvent


namespace Events
{
	EventResult EquipEventHandler::ReceiveEvent(RE::TESEquipEvent* a_event, EventDispatcher<RE::TESEquipEvent>* a_dispatcher)
	{
		UInt32 sourceFormID = a_event->akSource ? a_event->akSource->formID : 0;
		UInt32 playerFormID = g_thePlayer ? (*g_thePlayer)->formID : std::numeric_limits<UInt32>::min();
		if (sourceFormID != playerFormID) {
			return kEvent_Continue;
		}

		TESForm* form = LookupFormByID(a_event->formID);
		if (!form || form->formType != kFormType_Weapon) {
			return kEvent_Continue;
		}

		TESObjectWEAP* weap = static_cast<TESObjectWEAP*>(form);
		if ((weap->gameData.flags1 & TESObjectWEAP::GameData::kFlags_BoundWeapon) == 0) {
			return kEvent_Continue;
		}

		if (a_event->isEquipping) {
			UInt32 equipSlots = GetEquippedSlots((*g_thePlayer), weap);
			OnBoundWeaponEquippedRegSet::GetSingleton()->QueueEvent(weap->gameData.type, equipSlots);
		} else {
			UInt32 unequipSlots = GetUnequippedSlots((*g_thePlayer));
			OnBoundWeaponUnequippedRegSet::GetSingleton()->QueueEvent(weap, unequipSlots);
		}

		return kEvent_Continue;
	}


	EquipEventHandler* EquipEventHandler::GetSingleton()
	{
		static EquipEventHandler singleton;
		return &singleton;
	}
}
