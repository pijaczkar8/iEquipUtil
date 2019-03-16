#include "RE/Inventory.h"

#if _WIN64
#include "Relocation.h"  // RelocAddr
#endif


namespace RE
{
	EventDispatcher<Inventory::Event>* Inventory::GetEventSource()
	{
#if _WIN64
		typedef EventDispatcher<Inventory::Event>* _GetEventSource_t();
		static RelocAddr<_GetEventSource_t*> _GetEventSource(0x001EF330);
		return _GetEventSource();
#else
		return (EventDispatcher<Inventory::Event>*)0x012E5FB0;
#endif
	}
}
