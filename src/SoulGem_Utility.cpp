#include "SoulGem_Utility.h"

#include "skse64/GameTypes.h"
#include "skse64/PapyrusNativeFunctions.h"
#include "skse64/PapyrusVM.h"
#include "skse64/PluginAPI.h"
#include "skse64/GameReferences.h"
#include "skse64/GameExtraData.h"
#include "skse64/GameForms.h"

#include <string>
#include <sstream>
#include <iomanip>


namespace SoulGem_Utility
{
	std::string IntToHexString(int a_val)
	{
		// Convert to hex
		std::stringstream sstream;
		sstream << std::hex << a_val;
		std::string str = sstream.str();

		// Convert lowercase characters to uppercase
		for (auto & c : str) {
			c = toupper(c);
		}

		// Sign extension
		while (std::strlen(str.c_str()) < 8) {
			str = '0' + str;
		}

		return str;
	}

	void DumpSoulGemInfo(StaticFunctionTag* base)
	{
		_DMESSAGE("");
		_DMESSAGE("DumpSoulGemInfo Called!");
		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : NULL;
		if (!containerData) {
			_DMESSAGE("ERROR: No container data!");
			return;
		}
		UInt32 soulLevel;
		std::string str;
		InventoryEntryData* entryData = 0;
		for (UInt32 i = 0; i < containerData->objList->Count(); ++i) {
			entryData = containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formType == kFormType_SoulGem) {
					soulLevel = CALL_MEMBER_FN(entryData, GetSoulLevel)();
					str = "Entry [" + std::to_string(i) + "] : ";
					_DMESSAGE(str.c_str());
					_DMESSAGE("Is a soul gem");
					str = "(soul level == " + std::to_string(soulLevel) + ")";
					_DMESSAGE(str.c_str());
					str = "(formID == " + IntToHexString(entryData->type->formID) + ")";
					_DMESSAGE(str.c_str());
					str = "(countDelta == " + std::to_string(entryData->countDelta) + ")";
					_DMESSAGE(str.c_str());
					_DMESSAGE("");
				}
			} else {
				_DMESSAGE("ERROR: No entry data!");
			}
		}
	}


	bool RegisterFuncs(VMClassRegistry* registry)
	{
		registry->RegisterFunction(
			new NativeFunction0<StaticFunctionTag, void>("DumpSoulGemInfo", "SoulGem_Utility", SoulGem_Utility::DumpSoulGemInfo, registry));

		return true;
	}
}