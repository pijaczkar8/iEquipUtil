#include "RE/EventDispatcherList.h"


namespace RE
{
#ifndef _WIN64
	decltype(g_equipEventDispatcher) g_equipEventDispatcher = reinterpret_cast<decltype(g_equipEventDispatcher)>(0x012E4EA0);
	decltype(g_uniqueIDChangedEventDispatcher) g_uniqueIDChangedEventDispatcher = reinterpret_cast<decltype(g_uniqueIDChangedEventDispatcher)>(0x012E5560);
#endif
}
