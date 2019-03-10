#include "Events.h"

#undef min
#undef max

#include "GameAPI.h"  // g_thePlayer
#include "GameEvents.h"  // EventResult, EventDispatcher
#include "GameObjects.h"  // TESObjectWEAP, TESObjectARMO
#include "GameTypes.h"  // BSFixedString
#include "ITypes.h"  // UInt32, SInt32
#include "PapyrusArgs.h"  // PackValue()
#include "PapyrusEvents.h"  // EventRegistration, SKSEModCallbackEvent, NullParameters, RegistrationSetHolder, NullParameters
#include "PapyrusVM.h"  // Output, VMClassRegistry, IFunctionArguments

#include <limits>  // numeric_limits
#include <memory>  // shared_ptr

#include "ActorExtLib.h"  // GetEquippedHand
#include "Armor.h"  // Armor
#include "InventoryHandler.h"  // InventoryHandler
#include "SerializableFormFactory.h"  // SerializableFormFactory
#include "RE/BSTList.h"  // RE::BSSimpleList
#include "RE/TESEquipEvent.h"  // RE::TESEquipEvent


#if _WIN64
// Moved from PapyrusVM in SSE
#include "PapyrusValue.h"  // VMValue
#endif


namespace Events
{
	template <typename T> void SetVMValue(VMValue* a_val, T a_arg)
	{
		VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
		PackValue(a_val, &a_arg, registry);
	}


	template <> void SetVMValue<bool>(VMValue * val, bool arg) { val->SetBool(arg); }
	template <> void SetVMValue<SInt32>(VMValue * val, SInt32 arg) { val->SetInt(arg); }
	template <> void SetVMValue<float>(VMValue * val, float arg) { val->SetFloat(arg); }
	template <> void SetVMValue<BSFixedString>(VMValue * val, BSFixedString arg) { val->SetString(arg.data); }


	template <typename T1>
	class EventQueueFunctor1 : public IFunctionArguments
	{
	public:
		EventQueueFunctor1(BSFixedString& a_eventName, T1 a_arg1) :
			eventName(a_eventName.data),
			arg1(a_arg1)
		{}


		virtual bool Copy(Output* a_dst) override
		{
			a_dst->Resize(1);
			SetVMValue(a_dst->Get(0), arg1);

			return true;
		}


		void operator() (const EventRegistration<NullParameters>& a_reg)
		{
			VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
			registry->QueueEvent(a_reg.handle, &eventName, this);
		}


	private:
		BSFixedString eventName;
		T1 arg1;
	};


	template <typename T1, typename T2>
	class EventQueueFunctor2 : public IFunctionArguments
	{
	public:
		EventQueueFunctor2(BSFixedString & a_eventName, T1 a_arg1, T2 a_arg2) :
			eventName(a_eventName.data),
			arg1(a_arg1),
			arg2(a_arg2)
		{}

		virtual bool Copy(Output* a_dst)
		{
			a_dst->Resize(2);
			SetVMValue(a_dst->Get(0), arg1);
			SetVMValue(a_dst->Get(1), arg2);

			return true;
		}

		void operator() (const EventRegistration<NullParameters>& a_reg)
		{
			VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
			registry->QueueEvent(a_reg.handle, &eventName, this);
		}

	private:
		BSFixedString	eventName;
		T1				arg1;
		T2				arg2;
	};


	void PushInventoryEntry(InventoryEntryData* a_entryData)
	{
		Forms::FormFactory* formFactory = Forms::FormFactory::GetSingleton();
		Forms::InventoryHandler* invHandler = Forms::InventoryHandler::GetSingleton();
		SInt32 extraCount = 0;
		Forms::SerializableFormPtr form = nullptr;
		if (a_entryData->extendDataList) {
			RE::BSSimpleList<BaseExtraList*>* exDataList = (RE::BSSimpleList<BaseExtraList*>*)a_entryData->extendDataList;
			for (auto it = exDataList->begin(); it != exDataList->end(); ++it) {
				++extraCount;
				form = formFactory->GetForm(a_entryData->type->formType, true);
				if (form) {
					form->Set(a_entryData->type, *it);
					invHandler->AddForm(form, 1);
				}
			}
		}
		SInt32 baseCount = a_entryData->countDelta - extraCount;
		if (baseCount > 0) {
			form = formFactory->GetForm(a_entryData->type->formType, true);
			if (form) {
				form->Set(a_entryData->type, 0);
				invHandler->AddForm(form, baseCount);
			}
		}
		bool dummy = true;
	}


	EquipEventHandler::EquipEventHandler()
	{}


	EquipEventHandler::~EquipEventHandler()
	{}


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
			static BSFixedString callbackName = "OnBoundWeaponEquipped";
			UInt32 equipSlots = getEquippedSlots((*g_thePlayer), weap);
			g_boundWeaponEquippedCallbackRegs.ForEach(EventQueueFunctor2<UInt32, UInt32>(callbackName, weap->gameData.type, equipSlots));
			_DMESSAGE("[DEBUG] %s event dispatched\n", callbackName.data);
		} else {
			static BSFixedString callbackName = "OnBoundWeaponUnequipped";
			UInt32 unequipSlots = getUnequippedSlots((*g_thePlayer));
			g_boundWeaponUnequippedCallbackRegs.ForEach(EventQueueFunctor2<TESObjectWEAP*, UInt32>(callbackName, weap, unequipSlots));
			_DMESSAGE("[DEBUG] %s event dispatched\n", callbackName.data);
		}

		return kEvent_Continue;
	}


	EquipEventHandler* EquipEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new EquipEventHandler();
		}
		return _singleton;
	}


	void EquipEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	EquipEventHandler* EquipEventHandler::_singleton = 0;


	InventoryEventHandler::InventoryEventHandler()
	{}


	InventoryEventHandler::~InventoryEventHandler()
	{}


	EventResult InventoryEventHandler::ReceiveEvent(RE::Inventory::Event* a_event, EventDispatcher<RE::Inventory::Event>* a_dispatcher)
	{
		if (!a_event || !a_event->entryData || !a_event->entryData->type || !a_event->objRefr || a_event->objRefr->formID != (*g_thePlayer)->formID) {
			return kEvent_Continue;
		}

		PushInventoryEntry(a_event->entryData);

		return kEvent_Continue;
	}


	InventoryEventHandler* InventoryEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new InventoryEventHandler();
		}
		return _singleton;
	}


	void InventoryEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	InventoryEventHandler* InventoryEventHandler::_singleton = 0;


	ItemCraftedEventHandler::ItemCraftedEventHandler()
	{}


	ItemCraftedEventHandler::~ItemCraftedEventHandler()
	{}


	EventResult ItemCraftedEventHandler::ReceiveEvent(RE::ItemCrafted::Event* a_event, EventDispatcher<RE::ItemCrafted::Event>* a_dispatcher)
	{
		if (!a_event | !a_event->item) {
			return kEvent_Continue;
		}

		Forms::InventoryHandler* invHandler = Forms::InventoryHandler::GetSingleton();
		invHandler->GarbageCollectoFormList(a_event->item->formID);

		ExtraContainerChanges* changes = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
		if (changes && changes->data && changes->data->objList) {
			RE::BSSimpleList<InventoryEntryData*>* dataList = (RE::BSSimpleList<InventoryEntryData*>*)changes->data->objList;
			for (auto it = dataList->begin(); it != dataList->end(); ++it) {
				if ((*it)->type && (*it)->type->formID == a_event->item->formID) {
					PushInventoryEntry(*it);
					break;
				}
			}
		}

		bool dummy = true;

		return kEvent_Continue;
	}


	ItemCraftedEventHandler* ItemCraftedEventHandler::GetSingleton()
	{
		if (!_singleton) {
			_singleton = new ItemCraftedEventHandler();
		}
		return _singleton;
	}


	void ItemCraftedEventHandler::Free()
	{
		delete _singleton;
		_singleton = 0;
	}


	ItemCraftedEventHandler* ItemCraftedEventHandler::_singleton = 0;


	RegistrationSetHolder<NullParameters> g_boundWeaponEquippedCallbackRegs;
	RegistrationSetHolder<NullParameters> g_boundWeaponUnequippedCallbackRegs;
}
