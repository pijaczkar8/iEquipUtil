#pragma once

#include "GameExtraData.h"  // InventoryEntryData
#include "ITypes.h"  // UInt32

#include <vector>  // vector
#include <string>  // string


namespace iEquip_Utility
{
	extern bool GIST;
	extern SInt32 GISTIndex;


	bool checkForGIST();
	std::string boolToString(bool a_bool);
	std::string numToHexString(UInt64 a_num, UInt64 a_bytes = 4);


	class ExtraListLocator
	{
	public:
		ExtraListLocator(InventoryEntryData* a_entryData, std::vector<UInt32> a_whiteList, std::vector<UInt32> a_blackList);
		~ExtraListLocator();

		BaseExtraList* found();

	private:
		bool checkWhiteList(BaseExtraList* a_extraList);
		bool checkBlackList(BaseExtraList* a_extraList);

		InventoryEntryData* _entryData;
		std::vector<UInt32> _whiteList;
		std::vector<UInt32> _blackList;
		UInt32 _pos;
	};
}