#include "AmmoExt.h"

#include "GameForms.h"  // BGSKeyword, BGSDefaultObjectManager
#include "GameObjects.h"  // TESAmmo, BGSProjectile
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag

#include "Utility.h"  // GetForm


namespace iEquip
{
	bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			_ERROR("[ERROR] Invalid ammo!");
			return false;
		} else {
			BGSKeyword* WeapTypeBoundArrow = GetForm<BGSKeyword>(kSkyrim_WeapTypeBoundArrow, NAME_Skyrim, false);
			return a_ammo->keyword.HasKeyword(WeapTypeBoundArrow);
		}
	}


	bool IsAmmoJavelin(StaticFunctionTag* a_base, TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			_ERROR("[ERROR] Invalid ammo!\n");
			return false;
		}

		BGSKeyword* Javelin = GetForm<BGSKeyword>(kSpearsBySoolie_Javelin, NAME_SpearsBySoolie, false);
		if (!Javelin) {
			_WARNING("[WARNING] Failed to retrieve Javelin keyword!\n");
			return false;
		}

		return a_ammo->keyword.HasKeyword(Javelin);
	}


	UInt32 GetAmmoElement(StaticFunctionTag* a_base, TESAmmo* a_ammo)
	{
		if (!a_ammo) {
			_ERROR("[ERROR] Invalid ammo!\n");
		}

		BGSProjectile* projectile = a_ammo->settings.projectile;
		UInt32 formID = projectile ? projectile->formID : 0;
		_DMESSAGE("[DEBUG] formID == 0x%X", formID);
		UInt8 idx = formID >> (3 * 8);
		_DMESSAGE("[DEBUG] idx == 0x%X", idx);
		if (idx != 0xFE) {
			return to_underlying(Element::kNone);
		}

		UInt32 lightFormID = formID << ((2 * 8) + 4);
		_DMESSAGE("[DEBUG] lightForm == 0x%X", lightFormID);
		switch (lightFormID) {
		case kExoticArrows_ccBGSSSE002_ArrowTelekinesisProj01_TriggerExp:
			return to_underlying(Element::kTelekenesis);
		case kExoticArrows_ccBGSSSE002_ArrowSoulstealerProjectile:
			return to_underlying(Element::kSoulStealer);
		case kExoticArrows_ccBGSSSE002_ArrowFireProjectile:
			return to_underlying(Element::kFire);
		case kExoticArrows_ccBGSSSE002_ArrowIceProjectile:
			return to_underlying(Element::kIce);
		case kExoticArrows_ccBGSSSE002_ArrowLightningProjectile:
			return to_underlying(Element::kLightning);
		case kExoticArrows_ccBGSSSE002_ArrowBoneProjectile:
			return to_underlying(Element::kBone);
		default:
			return to_underlying(Element::kNone);
		}
	}


	namespace AmmoExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESAmmo*>("IsAmmoBound", "iEquip_AmmoExt", IsAmmoBound, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, bool, TESAmmo*>("IsAmmoJavelin", "iEquip_AmmoExt", IsAmmoJavelin, a_registry));

			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, UInt32, TESAmmo*>("GetAmmoElement", "iEquip_AmmoExt", GetAmmoElement, a_registry));

			return true;
		}
	}
}
