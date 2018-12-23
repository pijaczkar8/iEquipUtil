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

		if (formID == ccBGSSSE002_ArrowTelekinesisProj01_TriggerExp.GetLoadedFormID()) {
			return to_underlying(Element::kTelekenesis);
		} else if (formID == ccBGSSSE002_ArrowSoulstealerProjectile.GetLoadedFormID()) {
			return to_underlying(Element::kSoulStealer);
		} else if (formID == ccBGSSSE002_ArrowFireProjectile.GetLoadedFormID()) {
			return to_underlying(Element::kFire);
		} else if (formID == ccBGSSSE002_ArrowIceProjectile.GetLoadedFormID()) {
			return to_underlying(Element::kIce);
		} else if (formID == ccBGSSSE002_ArrowLightningProjectile.GetLoadedFormID()) {
			return to_underlying(Element::kLightning);
		} else if (formID == ccBGSSSE002_ArrowBoneProjectile.GetLoadedFormID()) {
			return to_underlying(Element::kBone);
		} else {
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
