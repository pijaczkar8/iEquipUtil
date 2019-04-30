#pragma once

#include "GameTypes.h"  // BSFixedString

class VMClassRegistry;
struct StaticFunctionTag;


namespace StringExt
{
	BSFixedString	LocalizeString(StaticFunctionTag*, BSFixedString a_str);
	UInt32			CalcCRC32Hash(StaticFunctionTag*, BSFixedString a_str, UInt32 a_start);
	BSFixedString	IntToHexString(StaticFunctionTag*, UInt32 a_num);
	bool			RegisterFuncs(VMClassRegistry* a_registry);
}
