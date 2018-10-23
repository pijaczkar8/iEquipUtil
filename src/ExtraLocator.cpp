#include "ExtraLocator.h"

#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // InventoryEntryData
#include "ITypes.h"  // UInt32

#include <vector>  // vector


namespace iEquip
{
	ExtraListLocator::ExtraListLocator(InventoryEntryData* a_entryData, std::vector<UInt32> a_whitelist, std::vector<UInt32> a_blacklist) :
		_entryData(a_entryData),
		_whiteList(a_whitelist),
		_blackList(a_blacklist),
		_pos(0)
	{}


	ExtraListLocator::~ExtraListLocator()
	{}


	BaseExtraList* ExtraListLocator::found()
	{
		BaseExtraList* extraList = 0;
		while (_pos < _entryData->extendDataList->Count()) {
			extraList = _entryData->extendDataList->GetNthItem(_pos++);
			if (extraList && checkWhiteList(extraList) && checkBlackList(extraList)) {
				return extraList;
			}
		}
		return 0;
	}


	void ExtraListLocator::setEntryData(InventoryEntryData* a_entryData)
	{
		_entryData = a_entryData;
		resetPos();
	}


	void ExtraListLocator::resetPos()
	{
		_pos = 0;
	}


	bool ExtraListLocator::checkWhiteList(BaseExtraList* a_extraList)
	{
		for (auto& type : _whiteList) {
			if (!a_extraList->HasType(type)) {
				return false;
			}
		}
		return true;
	}


	bool ExtraListLocator::checkBlackList(BaseExtraList* a_extraList)
	{
		for (auto& type : _blackList) {
			if (a_extraList->HasType(type)) {
				return false;
			}
		}
		return true;
	}
}