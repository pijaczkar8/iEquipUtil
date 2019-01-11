#include "StringExt.h"

#include "GameTypes.h"  // BSFixedString
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include "LocaleManager.h"  // LocaleManager


namespace iEquip
{
	BSFixedString LocalizeString(StaticFunctionTag* a_base, BSFixedString a_str)
	{
		if (!a_str.data) {
			_ERROR("[ERROR] String data was invalid!\n");
		}

		LocaleManager* locManager = LocaleManager::GetSingleton();
		return BSFixedString(locManager->GetLocalization(a_str.data).c_str());
	}


	namespace StringExt
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction1<StaticFunctionTag, BSFixedString, BSFixedString>("LocalizeString", "iEquip_StringExt", LocalizeString, a_registry));

			return true;
		}
	}
}
