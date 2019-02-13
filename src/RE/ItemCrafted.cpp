#include "RE/ItemCrafted.h"

#if _WIN64
#include "Relocation.h"  // RelocAddr
#endif


namespace RE
{
	EventDispatcher<ItemCrafted::Event>* ItemCrafted::GetEventSource()
	{
		typedef EventDispatcher<ItemCrafted::Event>* _GetEventSource_t();
#if _WIN64
		static RelocAddr<_GetEventSource_t*> _GetEventSource(0x00870A90);
#else
		static _GetEventSource_t* _GetEventSource = (_GetEventSource_t*)0x01B3E7F4;
#endif
		return _GetEventSource();
}
}
