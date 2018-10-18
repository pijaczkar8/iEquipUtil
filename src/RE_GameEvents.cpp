#include "RE_GameEvents.h"

#include "GameForms.h"  // LookupFormByID
#include "GameObjects.h"  // TESObjectWEAP
#include "GameRTTI.h"  // DYNAMIC_CAST

#include "iEquip_WeaponExt.h"  // IsWeaponBound()


namespace RE
{
	TESObjectWEAP* TESEquipEvent::checkIfBoundWeapEquipped()
	{
		if (!isUnequipEvent()) {
			TESForm* form = LookupFormByID(formID);
			if (form) {
				TESObjectWEAP* weap = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
				if (weap && weap->formType == kFormType_Weapon) {
					return iEquip_WeaponExt::IsWeaponBound(0, weap) ? weap : 0;
				}
			}
		}
		return 0;
	}


#if _WIN64
	bool TESEquipEvent::isUnequipEvent()
	{
		return unk10 == 0xFF000000;
	}

#else
	bool TESEquipEvent::isUnequipEvent()
	{
		return unk0C == 0x12000000;
	}


	EventDispatcher<TESEquipEvent>* g_equipEventDispatcher = (EventDispatcher<TESEquipEvent>*) 0x012E4EA0;

#endif
}