#include "RE_GameEvents.h"

#include "GameForms.h"  // LookupFormByID
#include "GameObjects.h"  // TESObjectWEAP
#include "GameRTTI.h"  // DYNAMIC_CAST

#include "WeaponExt.h"  // IsWeaponBound()


namespace RE
{
	TESObjectWEAP* TESEquipEvent::checkIfBoundWeapEquipEvent()
	{
		TESForm* form = LookupFormByID(formID);
		if (form && form->formType == kFormType_Weapon) {
			TESObjectWEAP* weap = static_cast<TESObjectWEAP*>(form);
			return iEquip::IsWeaponBound(0, weap) ? weap : 0;
		}
		return 0;
	}


#if _WIN64
	bool TESEquipEvent::isUnequipWeaponArmorEvent()
	{
		return unk10 == 0xFF000000;
	}

#else
	bool TESEquipEvent::isUnequipWeaponArmorEvent()
	{
		return unk0C == 0x12000000;
	}


	EventDispatcher<TESEquipEvent>* g_equipEventDispatcher = (EventDispatcher<TESEquipEvent>*) 0x012E4EA0;

#endif
}