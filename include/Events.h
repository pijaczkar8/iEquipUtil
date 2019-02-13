#pragma once

#include "GameEvents.h"  // BSTEventSink, EventResult, EventDispatcher
#include "PapyrusEvents.h"  // SKSEModCallbackEvent, RegistrationSetHolder, NullParameters
#include "RE/TESEquipEvent.h"  // RE::TESEquipEvent
#include "RE/Inventory.h"  // RE::Inventory
#include "RE/ItemCrafted.h"  // RE::ItemCrafted


namespace iEquip
{
	void PushInventoryEntry(InventoryEntryData* a_entryData);


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


		static EquipEventHandler* _singleton;
	};


	class InventoryEventHandler : public BSTEventSink<RE::Inventory::Event>
	{
	protected:
		InventoryEventHandler();
		virtual ~InventoryEventHandler();

	public:
		virtual EventResult ReceiveEvent(RE::Inventory::Event* a_event, EventDispatcher<RE::Inventory::Event>* a_dispatcher) override;

		static InventoryEventHandler* GetSingleton();
		static void Free();

	private:
		InventoryEventHandler(const InventoryEventHandler&) = delete;
		InventoryEventHandler(InventoryEventHandler&&) = delete;

		InventoryEventHandler&	operator=(const InventoryEventHandler&) = delete;
		InventoryEventHandler&	operator=(InventoryEventHandler&&) = delete;


		static InventoryEventHandler* _singleton;
	};


	class ItemCraftedEventHandler : public BSTEventSink<RE::ItemCrafted::Event>
	{
	protected:
		ItemCraftedEventHandler();
		virtual ~ItemCraftedEventHandler();

	public:
		virtual EventResult ReceiveEvent(RE::ItemCrafted::Event* a_event, EventDispatcher<RE::ItemCrafted::Event>* a_dispatcher) override;

		static ItemCraftedEventHandler* GetSingleton();
		static void Free();

	private:
		ItemCraftedEventHandler(const ItemCraftedEventHandler&) = delete;
		ItemCraftedEventHandler(ItemCraftedEventHandler&&) = delete;

		ItemCraftedEventHandler&	operator=(const ItemCraftedEventHandler&) = delete;
		ItemCraftedEventHandler&	operator=(ItemCraftedEventHandler&&) = delete;


		static ItemCraftedEventHandler* _singleton;
	};


	extern RegistrationSetHolder<NullParameters> g_boundWeaponEquippedCallbackRegs;
	extern RegistrationSetHolder<NullParameters> g_boundWeaponUnequippedCallbackRegs;
}
