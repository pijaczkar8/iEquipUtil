#pragma once

class TESForm;
class TESObjectREFR;
class VMClassRegistry;
struct StaticFunctionTag;


namespace ObjectReferenceExt
{
	SInt32		GetNumItemsOfType(StaticFunctionTag*, TESObjectREFR* a_container, UInt32 a_type);
	TESForm*	GetNthFormOfType(StaticFunctionTag*, TESObjectREFR* a_container, UInt32 a_type, UInt32 a_n);

	bool RegisterFuncs(VMClassRegistry* a_registry);
}
