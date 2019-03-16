#pragma once

class TESObjectREFR;
class VMClassRegistry;
struct StaticFunctionTag;


TESObjectREFR* GetObjectReferenceAtInventoryIndex(StaticFunctionTag* a_base, UInt32 a_index);


namespace UIExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
