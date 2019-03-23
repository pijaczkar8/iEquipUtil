#include "Registration.h"

#include "GameTypes.h"  // BSFixedString
#include "PapyrusArgs.h"  // PackValue
#include "PapyrusVM.h"  // VMClassRegistry, IFunctionArguments


namespace
{
	template<class T> void SetVMValue(VMValue* a_val, T a_arg)
	{
		VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
		PackValue(a_val, &a_arg, registry);
	}


	template<> void SetVMValue<bool>(VMValue* a_val, bool a_arg) { a_val->SetBool(a_arg); }
	template<> void SetVMValue<SInt32>(VMValue* a_val, SInt32 a_arg) { a_val->SetInt(a_arg); }
	template<> void SetVMValue<float>(VMValue* a_val, float a_arg) { a_val->SetFloat(a_arg); }
	template<> void SetVMValue<const BSFixedString&>(VMValue* a_val, const BSFixedString& a_arg) { a_val->SetString(a_arg.data); }


	template <class T1, class T2, class T3, class T4, class T5>
	class EventQueueFunctor5 : public IFunctionArguments
	{
	public:
		EventQueueFunctor5(BSFixedString& a_eventName, T1 a_arg1, T2 a_arg2, T3 a_arg3, T4 a_arg4, T5 a_arg5) :
			_eventName(a_eventName.data),
			_arg1(a_arg1),
			_arg2(a_arg2),
			_arg3(a_arg3),
			_arg4(a_arg4),
			_arg5(a_arg5)
		{}


		~EventQueueFunctor5()
		{
			CALL_MEMBER_FN(&_eventName, Release)();
		}


		virtual bool Copy(Output* a_dst) override
		{
			a_dst->Resize(5);
			SetVMValue(a_dst->Get(0), _arg1);
			SetVMValue(a_dst->Get(1), _arg2);
			SetVMValue(a_dst->Get(2), _arg3);
			SetVMValue(a_dst->Get(3), _arg4);
			SetVMValue(a_dst->Get(4), _arg5);
			return true;
		}


		void operator()(const EventRegistration<NullParameters>& a_reg)
		{
			VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();
			registry->QueueEvent(a_reg.handle, &_eventName, this);
		}

	private:
		BSFixedString	_eventName;
		T1				_arg1;
		T2				_arg2;
		T3				_arg3;
		T4				_arg4;
		T5				_arg5;
	};
}


OnPlayerItemAddedRegSet* OnPlayerItemAddedRegSet::GetSingleton()
{
	if (!_singleton) {
		_singleton = new OnPlayerItemAddedRegSet();
	}
	return _singleton;
}


void OnPlayerItemAddedRegSet::Free()
{
	delete _singleton;
	_singleton = 0;
}


void OnPlayerItemAddedRegSet::SendEvent(TESForm* a_baseItem, SInt32 a_itemCount, TESObjectREFR* a_itemReference, TESObjectREFR* a_sourceContainer, const BSFixedString& a_name)
{
	static BSFixedString callback = "OnPlayerItemAdded";
	ForEach(EventQueueFunctor5<TESForm*, SInt32, TESObjectREFR*, TESObjectREFR*, BSFixedString>(callback, a_baseItem, a_itemCount, a_itemReference, a_sourceContainer, a_name));
}


OnPlayerItemAddedRegSet::OnPlayerItemAddedRegSet()
{}
