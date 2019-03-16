#include "RE/ItemCrafted.h"

#if _WIN64
#include "Relocation.h"  // RelocAddr
#endif


namespace RE
{
	EventDispatcher<ItemCrafted::Event>* ItemCrafted::GetEventSource()
	{
#if _WIN64
		typedef EventDispatcher<ItemCrafted::Event>* _GetEventSource_t();
		static RelocAddr<_GetEventSource_t*> _GetEventSource(0x008708A0);
		return _GetEventSource();
#else
		return (EventDispatcher<ItemCrafted::Event>*)0x01B3E7F4;
#endif
	}
}
