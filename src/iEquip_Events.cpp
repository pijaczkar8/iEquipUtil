#include "iEquip_Events.h"

#include "GameEvents.h"  // EventResult, EventDispatcher
#include "GameTypes.h"  // BSFixedString
#include "ITypes.h"  // UInt32, SInt32
#include "PapyrusArgs.h"  // PackValue()
#include "PapyrusEvents.h"  // EventRegistration, SKSEModCallbackEvent, NullParameters, RegistrationSetHolder, NullParameters
#include "PapyrusValue.h"  // VMValue
#include "PapyrusVM.h"  // Output, VMClassRegistry, IFunctionArguments

#include "RE_GameEvents.h"  // RE::TESEquipEvent


namespace iEquip_Events
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


	EventResult EquipEventHandler::ReceiveEvent(RE::TESEquipEvent* a_event, EventDispatcher<RE::TESEquipEvent>* a_dispatcher)
	{
		if (a_event->akSource != *g_thePlayer) {
			return kEvent_Continue;
		} else if (a_event->checkIfBoundWeapEquipped()) {
			static BSFixedString callbackName = "OnBoundWeaponEquipped";
			g_callbackRegs.ForEach(
				EventQueueFunctor1<UInt32>(callbackName, a_event->getWeaponType())
			);
			_DMESSAGE("[DEBUG] BoundWeaponEquipped event dispatched\n");
		}
		return kEvent_Continue;
	}


	RegistrationSetHolder<NullParameters> g_callbackRegs;
	EventDispatcher<SKSEModCallbackEvent>* g_callbackDispatcher = 0;
	EquipEventHandler g_equipEventHandler;
}