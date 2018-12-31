#pragma once

struct StaticFunctionTag;
class TESAmmo;
class VMClassRegistry;


namespace iEquip
{
	bool IsAmmoBound(StaticFunctionTag* a_base, TESAmmo* a_ammo);


	namespace AmmoExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}
