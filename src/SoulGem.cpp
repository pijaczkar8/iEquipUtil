#include "SoulGem.h"

#include "common/IDebugLog.h"  // gLog
#include "common/ITypes.h"  // UInt32
#include "skse64/GameExtraData.h"  // InventoryEntryData

#include <vector>  // vector


namespace iEquip_SoulSeeker
{
	Gem::Gem(UInt32 a_soulGemType) :
		soulGemType(a_soulGemType)
	{}


	bool Gem::empty()
	{
		return entryData.empty();
	}


	bool Gem::addGem(InventoryEntryData* a_entry)
	{
		entryData.push_back(a_entry);
		return true;
	}


	Soul::Soul(UInt32 a_soulSize) :
		soulSize(a_soulSize)
	{
		switch (soulSize) {
		case kSoulSize_Petty:
			compatGems.emplace_back(kSoulGem_PettyFilled);
			compatGems.emplace_back(kSoulGem_Petty);
		case kSoulSize_Lesser:
			compatGems.emplace_back(kSoulGem_LesserFilled);
			compatGems.emplace_back(kSoulGem_Lesser);
		case kSoulSize_Common:
			compatGems.emplace_back(kSoulGem_CommonFilled);
			compatGems.emplace_back(kSoulGem_Common);
		case kSoulSize_Greater:
			compatGems.emplace_back(kSoulGem_GreaterFilled);
			compatGems.emplace_back(kSoulGem_Greater);
		case kSoulSize_Grand:
			compatGems.emplace_back(kSoulGem_GrandFilled);
			compatGems.emplace_back(kSoulGem_Grand);
			compatGems.emplace_back(kSoulGem_BlackFilled);
			compatGems.emplace_back(kSoulGem_Black);
			compatGems.emplace_back(kSoulGem_AzurasStar);
			compatGems.emplace_back(kSoulGem_BlackStar);
			break;
		default:
			_ERROR("ERROR: In Soul::Soul : Invalid soul level!");
		}
	}


	bool Soul::empty()
	{
		for (auto& gem : compatGems) {
			if (!gem.empty()) {
				return false;
			}
		}
		return true;
	}


	bool Soul::addSoul(InventoryEntryData* a_entry)
	{
		for (auto& gem : compatGems) {
			if (gem.soulGemType == a_entry->type->formID) {
				return gem.addGem(a_entry);
			}
		}
		_ERROR("ERROR: In Soul::addSoul : Invalid soulgem type!");
		return false;
	}


	InventoryEntryData* Soul::findGem(bool a_partialFill)
	{
		if (!empty()) {
			UInt32 max = a_partialFill ? compatGems.size() : getMaxFillCount(soulSize);
			InventoryEntryData* entry = 0;
			for (UInt32 i = 0; i < max; ++i) {
				if (!compatGems[i].empty()) {
					entry = compatGems[i].entryData.back();
					compatGems[i].entryData.pop_back();
					return entry;
				}
			}
			_ERROR("ERROR: In Soul::findGem : Unexpectedly exited for loop!");
		}
		return 0;
	}


	SoulGem::SoulGem()
	{
		souls.emplace_back(kSoulSize_Petty);
		souls.emplace_back(kSoulSize_Lesser);
		souls.emplace_back(kSoulSize_Common);
		souls.emplace_back(kSoulSize_Greater);
		souls.emplace_back(kSoulSize_Grand);
	}


	bool SoulGem::empty()
	{
		for (auto& soul : souls) {
			if (!soul.empty()) {
				return false;
			}
		}
		return true;
	}


	bool SoulGem::addSoulGem(InventoryEntryData* a_entry, UInt32 a_soulSize)
	{
		for (auto& soul : souls) {
			if (a_soulSize == soul.soulSize) {
				return soul.addSoul(a_entry);
			}
		}
		_ERROR("ERROR: In SoulGem::addSoulGem : Invalid soul level!");
		return false;
	}


	InventoryEntryData* SoulGem::findSoul(UInt32 a_soulSize, bool a_partialFill)
	{
		return empty() ? 0 : souls[a_soulSize - 1].findGem(a_partialFill);
	}


	UInt32 getMaxFillCount(UInt32 a_soulSize)
	{
		switch (a_soulSize) {
		case kSoulSize_Petty:
			return 2;
		case kSoulSize_Lesser:
			return 2;
		case kSoulSize_Common:
			return 2;
		case kSoulSize_Greater:
			return 2;
		case kSoulSize_Grand:
			return 6;
		default:
			return 0;
		}
	}
}