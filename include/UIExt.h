#pragma once

#include "GameTypes.h"  // BSFixedString

class TESForm;
class VMClassRegistry;
struct StaticFunctionTag;


TESForm* GetFormAtInventoryIndex(StaticFunctionTag* a_base, UInt32 a_index);


namespace UIExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
