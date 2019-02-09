#include "RE/ItemCrafted.h"

#include "Relocation.h"  // RelocAddr


namespace RE
{
	EventDispatcher<ItemCrafted::Event>* ItemCrafted::GetEventSource()
	{
		typedef EventDispatcher<ItemCrafted::Event>* _GetEventSource_t();
		static RelocAddr<_GetEventSource_t*> _GetEventSource(0x00870A90);
		return _GetEventSource();
	}
}
