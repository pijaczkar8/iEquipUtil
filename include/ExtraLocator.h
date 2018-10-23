#pragma once

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // InventoryEntryData
#include "ITypes.h"  // UInt32

#include <vector>  // vector


namespace iEquip
{
	class ExtraListLocator
	{
	public:
		ExtraListLocator(InventoryEntryData* a_entryData, std::vector<UInt32> a_whiteList, std::vector<UInt32> a_blackList);
		~ExtraListLocator();

		BaseExtraList* found();
		void setEntryData(InventoryEntryData* a_entryData);
		void resetPos();

	private:
		bool checkWhiteList(BaseExtraList* a_extraList);
		bool checkBlackList(BaseExtraList* a_extraList);

		InventoryEntryData* _entryData;
		std::vector<UInt32> _whiteList;
		std::vector<UInt32> _blackList;
		UInt32 _pos;
	};
}