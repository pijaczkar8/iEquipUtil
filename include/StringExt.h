#pragma once

#include "GameTypes.h"  // BSFixedString

class VMClassRegistry;
struct StaticFunctionTag;


BSFixedString	LocalizeString(StaticFunctionTag*, BSFixedString a_str);
UInt32			CalcCRC32Hash(StaticFunctionTag*, BSFixedString a_str, UInt32 a_start);
BSFixedString	IntToHexString(StaticFunctionTag*, UInt32 a_num);


namespace StringExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
