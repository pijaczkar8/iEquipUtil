#pragma once

struct StaticFunctionTag;
class TESAmmo;
class VMClassRegistry;


namespace AmmoExt
{
	bool	IsAmmoBound(StaticFunctionTag*, TESAmmo* a_ammo);
	bool	RegisterFuncs(VMClassRegistry* a_registry);
}
