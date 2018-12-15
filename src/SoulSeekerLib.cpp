#include "SoulSeekerLib.h"

#include "IDebugLog.h"  // gLog
#include "ITypes.h"  // UInt8, UInt32
#include "GameExtraData.h"  // InventoryEntryData
#include "GameForms.h"  // BGSKeyword, BGSDefaultObjectManager
#include "GameObjects.h"  // TESSoulGem

#include <vector>  // vector
#include <exception>  // exception
#include <string>  // string


namespace iEquip
{
	Gem::Gem()
	{}


	bool Gem::empty()
	{
		return _entryData.empty();
	}


	bool Gem::addGem(InventoryEntryData* a_entryData)
	{
		_entryData.push_back(a_entryData);
		return true;
	}


	InventoryEntryData* Gem::getEntryData()
	{
		InventoryEntryData* tmp = _entryData.back();
		_entryData.pop_back();
		return tmp;
	}


	Soul::Soul(UInt8 a_soulSize)
	{
		for (int i = 0; i < kSoulSize_Grand - a_soulSize + 1; ++i) {
			_compatGems.emplace_back();
		}
	}


	bool Soul::empty()
	{
		for (auto& gem : _compatGems) {
			if (!gem.empty()) {
				return false;
			}
		}
		return true;
	}


	bool Soul::addSoul(InventoryEntryData* a_entryData)
	{
		TESSoulGem* gem = static_cast<TESSoulGem*>(a_entryData->type);
		try {
			return _compatGems.at(kSoulSize_Grand - gem->gemSize).addGem(a_entryData);
		} catch (std::exception& e) {
			_ERROR("[ERROR] Invalid soulgem size! (%i)\n", gem->gemSize);
			_ERROR(e.what());
			_ERROR("\n");
		}
		return false;
	}


	InventoryEntryData* Soul::findGem(bool a_partialFill)
	{
		if (!empty()) {
			UInt32 min = a_partialFill ? 0 : _compatGems.size() - 1;
			for (UInt32 i = _compatGems.size() - 1; i >= min; --i) {
				if (!_compatGems[i].empty()) {
					return _compatGems[i].getEntryData();
				}
			}
			_ERROR("[ERROR] Unexpectedly exited for loop!\n");
		}
		return 0;
	}


	SoulGem::SoulGem()
	{
		_souls.emplace_back(kSoulSize_Petty);
		_souls.emplace_back(kSoulSize_Lesser);
		_souls.emplace_back(kSoulSize_Common);
		_souls.emplace_back(kSoulSize_Greater);
		_souls.emplace_back(kSoulSize_Grand);
	}


	bool SoulGem::empty()
	{
		for (auto& soul : _souls) {
			if (!soul.empty()) {
				return false;
			}
		}
		return true;
	}


	bool SoulGem::addSoulGem(InventoryEntryData* a_entryData, UInt8 a_soulSize)
	{
		try {
			return _souls.at(a_soulSize - 1).addSoul(a_entryData);
		} catch (std::exception& e ) {
			_ERROR("[ERROR] Invalid soul size! (%i)", a_soulSize - 1);
			_ERROR(e.what());
			_ERROR("\n");
			return false;
		}
	}


	InventoryEntryData* SoulGem::findSoul(UInt8 a_soulSize, bool a_partialFill)
	{
		return empty() ? 0 : _souls[a_soulSize - 1].findGem(a_partialFill);
	}


	bool isReusable(TESSoulGem* a_gem)
	{
		static BGSKeyword* reusableSoulGem = static_cast<BGSKeyword*>(BGSDefaultObjectManager::GetSingleton()->objects[0xD8]);
		return a_gem->keyword.HasKeyword(reusableSoulGem);
	}
}
