#include "RE/Inventory.h"

#if _WIN64
#include "Relocation.h"  // RelocAddr
#endif


namespace RE
{
	EventDispatcher<Inventory::Event>* Inventory::GetEventSource()
	{
		typedef EventDispatcher<Inventory::Event>* _GetEventSource_t();
#if _WIN64
		static RelocAddr<_GetEventSource_t*> _GetEventSource(0x001EF520);
#else
		static _GetEventSource_t* _GetEventSource = (_GetEventSource_t*)0x012E5FB0;
#endif
		return _GetEventSource();
}
}
