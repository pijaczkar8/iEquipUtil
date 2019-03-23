#pragma once

#include "GameTypes.h"  // BSFixedString

class TESObjectREFR;
class VMClassRegistry;
struct StaticFunctionTag;


BSFixedString GetDisplayName(StaticFunctionTag* a_base, TESObjectREFR* a_obj);


namespace ObjectReferenceExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
