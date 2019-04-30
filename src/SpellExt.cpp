#include "SpellExt.h"

#include "GameObjects.h"  // SpellItem, MagicItem::EffectItem, EffectSetting, TESObjectWEAP
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry


namespace SpellExt
{
	bool IsHealingSpell(StaticFunctionTag*, SpellItem* a_spell)
	{
		if (!a_spell) {
			_WARNING("[WARNING] a_spell is a NONE form!");
			return false;
		}

		MagicItem::EffectItem* effectItem = 0;
		EffectSetting* effectSetting = 0;
		for (UInt32 i = 0; i < a_spell->effectItemList.count; ++i) {
			effectItem = a_spell->effectItemList[i];
			if (effectItem && effectItem->mgef) {
				effectSetting = effectItem->mgef;
				if (effectSetting->properties.school == kActorValue_Restoration && effectSetting->properties.primaryValue == kActorValue_Health) {
					return true;
				}
			}
		}

		return false;
	}


	bool IsBoundSpell(StaticFunctionTag*, SpellItem* a_spell)
	{
		if (!a_spell) {
			_WARNING("[WARNING] a_spell is a NONE form!");
			return false;
		}

		MagicItem::EffectItem* effectItem = 0;
		EffectSetting* effectSetting = 0;
		for (UInt32 i = 0; i < a_spell->effectItemList.count; ++i) {
			effectItem = a_spell->effectItemList[i];
			if (effectItem && effectItem->mgef) {
				effectSetting = effectItem->mgef;
				if (effectSetting->properties.school == kActorValue_Conjuration && effectSetting->properties.archetype == EffectSetting::Properties::kArchetype_BoundWeapon) {
					return true;
				}
			}
		}

		return false;
	}


	SInt32 GetBoundSpellWeapType(StaticFunctionTag*, SpellItem* a_spell)
	{
		if (!a_spell) {
			_WARNING("[WARNING] a_spell is a NONE form!");
			return -1;
		}

		MagicItem::EffectItem* effectItem = 0;
		EffectSetting* effectSetting = 0;
		for (UInt32 i = 0; i < a_spell->effectItemList.count; ++i) {
			effectItem = a_spell->effectItemList[i];
			if (effectItem && effectItem->mgef) {
				effectSetting = effectItem->mgef;
				if (effectSetting->properties.school == kActorValue_Conjuration && effectSetting->properties.archetype == EffectSetting::Properties::kArchetype_BoundWeapon) {
					if (effectSetting->properties.primaryObject && effectSetting->properties.primaryObject->formType == kFormType_Weapon) {
						TESObjectWEAP* weap = static_cast<TESObjectWEAP*>(effectSetting->properties.primaryObject);
						if ((weap->gameData.flags1 & TESObjectWEAP::GameData::kFlags_BoundWeapon) != 0) {
							return weap->gameData.type;
						}
					}
				}
			}
		}

		return -1;
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, SpellItem*>("IsHealingSpell", "iEquip_SpellExt", IsHealingSpell, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, bool, SpellItem*>("IsBoundSpell", "iEquip_SpellExt", IsBoundSpell, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, SInt32, SpellItem*>("GetBoundSpellWeapType", "iEquip_SpellExt", GetBoundSpellWeapType, a_registry));

		return true;
	}
}
