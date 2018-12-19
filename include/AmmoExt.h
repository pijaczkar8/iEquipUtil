#pragma once

struct StaticFunctionTag;
class TESAmmo;
class VMClassRegistry;


namespace iEquip
{
	enum class Element : UInt32
	{
		kNone = 0,
		kTelekenesis = 1,
		kSoulStealer = 2,
		kFire = 3,
		kIce = 4,
		kLightning = 5,
		kBound = 6,
		kBone = 7
	};


	bool	IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo);
	bool	IsAmmoJavelin(StaticFunctionTag* a_base, TESAmmo* a_ammo);
	UInt32	GetAmmoElement(StaticFunctionTag* a_base, TESAmmo* a_ammo);


	namespace AmmoExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}
