#pragma once

#include "GameTypes.h"  // BSFixedString

class VMClassRegistry;
struct StaticFunctionTag;


BSFixedString LocalizeString(StaticFunctionTag* a_base, BSFixedString a_str);


namespace StringExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
