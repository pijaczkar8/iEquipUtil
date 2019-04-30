#pragma once

#include "GameTypes.h"  // BSFixedString

class TESForm;
class VMClassRegistry;
struct StaticFunctionTag;


namespace UIExt
{
	TESForm*	GetFormAtInventoryIndex(StaticFunctionTag*, UInt32 a_index);
	bool		RegisterFuncs(VMClassRegistry* a_registry);
}
