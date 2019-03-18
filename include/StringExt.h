#pragma once

#include "GameTypes.h"  // BSFixedString

class VMClassRegistry;
struct StaticFunctionTag;


BSFixedString	LocalizeString(StaticFunctionTag* a_base, BSFixedString a_str);
UInt32			CalcCRC32Hash(StaticFunctionTag* a_base, BSFixedString a_str, UInt32 a_start);


namespace StringExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
