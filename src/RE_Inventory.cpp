#include "RE_Inventory.h"

#include "Relocation.h"  // RelocAddr


namespace RE
{
	EventDispatcher<Inventory::Event>* Inventory::GetEventSource()
	{
		typedef EventDispatcher<Inventory::Event>* _GetEventSource_t();
		static RelocAddr<_GetEventSource_t*> _GetEventSource(0x001EF520);
		return _GetEventSource();
	}
}
