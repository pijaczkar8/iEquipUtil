#include "StringExt.h"

#include "GameTypes.h"  // BSFixedString
#include "HashUtil.h"  // CRC32
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <cstdio>  // printf
#include <cstring>  // strlen

#include "LocaleManager.h"  // LocaleManager


BSFixedString LocalizeString(StaticFunctionTag*, BSFixedString a_str)
{
	if (!a_str.data) {
		_ERROR("[ERROR] String data was invalid!\n");
	}

	LocaleManager* locManager = LocaleManager::GetSingleton();
	return BSFixedString(locManager->GetLocalization(a_str.data).c_str());
}


UInt32 CalcCRC32Hash(StaticFunctionTag*, BSFixedString a_str, UInt32 a_start)
{
	return HashUtil::CRC32(a_str.data, a_start);
}


BSFixedString IntToHexString(StaticFunctionTag*, UInt32 a_num)
{
	char buf[9] = { '\0' };
	std::snprintf(buf, sizeof(buf), "%08X", a_num);
	return buf;
}


namespace StringExt
{
	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("LocalizeString", "iEquip_StringExt", LocalizeString, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, UInt32, BSFixedString, UInt32>("CalcCRC32Hash", "iEquip_StringExt", CalcCRC32Hash, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction1<StaticFunctionTag, BSFixedString, UInt32>("IntToHexString", "iEquip_StringExt", IntToHexString, a_registry));

		return true;
	}
}
