#include "RE_GameEvents.h"

#include "GameObjects.h"  // TESObjectWEAP

#include "iEquip_WeaponExt.h"  // IsWeaponBound()


namespace RE
{
	bool TESEquipEvent::checkIfBoundWeapEquipped()
	{
		if (unk10 != 0xFF000000 && unk60 == 0x1) {
			if (unk28->formType == kFormType_Weapon) {
				TESObjectWEAP* weap = static_cast<TESObjectWEAP*>(unk28);
				return iEquip_WeaponExt::IsWeaponBound(0, weap);
			}
		}
		return 0;
	}


	UInt8 TESEquipEvent::getWeaponType()
	{
		TESObjectWEAP* weap = static_cast<TESObjectWEAP*>(unk28);
		return weap->gameData.type;
	}
}