#pragma once

#include "common/ITypes.h"  // UInt8, UInt32
#include "skse64/GameExtraData.h"  // InventoryEntryData

#include <vector> // vector


namespace iEquip_SoulSeeker
{
	enum SoulSize : UInt8
	{
		kSoulSize_Empty,
		kSoulSize_Petty,
		kSoulSize_Lesser,
		kSoulSize_Common,
		kSoulSize_Greater,
		kSoulSize_Grand
	};


	// Player filled soulgems have the same ID as unfilled soulgems
	// I might have to refactor this to account for load order, I don't know what happens if someone edits the soulgems.
	enum SoulGemType : UInt32
	{
		kSoulGem_Petty = 0x0002E4E2,
		kSoulGem_PettyFilled = 0x0002E4E3,
		kSoulGem_Lesser = 0x0002E4E4,
		kSoulGem_LesserFilled = 0x0002E4E5,
		kSoulGem_Common = 0x0002E4E6,
		kSoulGem_CommonFilled = 0x0002E4F3,
		kSoulGem_Greater = 0x0002E4F4,
		kSoulGem_GreaterFilled = 0x0002E4FB,
		kSoulGem_Grand = 0x0002E4FC,
		kSoulGem_GrandFilled = 0x0002E4FF,
		kSoulGem_Black = 0x0002E500,
		kSoulGem_BlackFilled = 0x0002E504,
		kSoulGem_AzurasStar = 0x00063B27,
		kSoulGem_BlackStar = 0x00063B29
	};


	class Gem
	{
	public:
		explicit Gem(UInt32 a_soulGemType);
		bool empty();
		bool addGem(InventoryEntryData* a_entry);

		UInt32 soulGemType;
		std::vector<InventoryEntryData*> entryData;
	};


	class Soul
	{
	public:
		explicit Soul(UInt32 a_soulSize);
		bool empty();
		bool addSoul(InventoryEntryData* a_entry);
		InventoryEntryData* findGem(bool a_partialFill);

		UInt32 soulSize;
		std::vector<Gem> compatGems;
	};


	class SoulGem
	{
	public:
		SoulGem();
		bool empty();
		bool addSoulGem(InventoryEntryData* a_entry, UInt32 a_soulSize);
		InventoryEntryData* findSoul(UInt32 a_soulSize, bool a_partialFill);

		std::vector<Soul> souls;
	};


	UInt32 getMaxFillCount(UInt32 a_soulSize);
}