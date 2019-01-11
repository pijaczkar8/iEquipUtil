#pragma once

#include "GameTypes.h"  // BSFixedString

class VMClassRegistry;
struct StaticFunctionTag;


namespace iEquip
{
	BSFixedString LocalizeString(StaticFunctionTag* a_base, BSFixedString a_str);


	namespace StringExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}
