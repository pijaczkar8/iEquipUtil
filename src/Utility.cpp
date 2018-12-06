#include "Utility.h"

#include "ITypes.h"  // UInt32, UInt64
#include "GameData.h"  // DataHandler
#include "GameForms.h"  // BGSKeyword, LookupFormByID, TESForm
#include "GameRTTI.h"  // DYNAMIC_CAST

#include <cctype>  // toupper
#include <ios>  // hex
#include <sstream>  // stringstream
#include <string>  // string, strlen


namespace iEquip
{
	std::string boolToString(bool a_bool)
	{
		return a_bool ? "True" : "False";
	}


	std::string numToHexString(UInt64 a_num, UInt64 a_bytes)
	{
		// Convert to hex
		std::stringstream sstream;
		sstream << std::hex << a_num;
		std::string hexStr = sstream.str();

		// Convert lowercase characters to uppercase
		for (auto& c : hexStr) {
			c = toupper(c);
		}

		// Sign extension
		while (std::strlen(hexStr.c_str()) < a_bytes * 2) {
			hexStr = '0' + hexStr;
		}

		return hexStr;
	}


	BGSKeyword* GetKeyword(Keyword a_keyword, std::string a_modName)
	{
		static DataHandler* dataHandler = DataHandler::GetSingleton();
		const ModInfo* info = dataHandler->LookupModByName(a_modName.c_str());
		UInt32 index = info->modIndex;
		UInt32 keywordFormID = (index * 0x1000000) + a_keyword;
		if (index != 0xFF) {
			BGSKeyword* keyword = DYNAMIC_CAST(LookupFormByID(keywordFormID), TESForm, BGSKeyword);
			return keyword;
		} else {
			return 0;
		}
	}
}
