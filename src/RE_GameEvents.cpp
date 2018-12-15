#include "RE_GameEvents.h"

#include "GameForms.h"  // LookupFormByID
#include "GameObjects.h"  // TESObjectWEAP
#include "GameRTTI.h"  // DYNAMIC_CAST

#include "WeaponExt.h"  // IsWeaponBound()


namespace RE
{
#if _WIN64
#else

	EventDispatcher<TESEquipEvent>* g_equipEventDispatcher = (EventDispatcher<TESEquipEvent>*) 0x012E4EA0;

#endif
}
