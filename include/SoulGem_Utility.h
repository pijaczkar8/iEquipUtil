#pragma once

#include "skse64/GameTypes.h"
#include "skse64/PapyrusNativeFunctions.h"
#include "skse64/PapyrusVM.h"
#include "skse64/PluginAPI.h"
#include <string>


namespace SoulGem_Utility
{
	std::string IntToHexString(int a_val);

	void DumpSoulGemInfo(StaticFunctionTag* base);

	bool RegisterFuncs(VMClassRegistry* registry);

	enum SoulSize : UInt8
	{
		kSoulSize_Empty = 0,
		kSoulSize_Petty = 1,
		kSoulSize_Lesser,
		kSoulSize_Common,
		kSoulSize_Greater,
		kSoulSize_Grand,
	};
}