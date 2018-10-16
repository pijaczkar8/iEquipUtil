#pragma once

#include "ITypes.h"  // UInt8
#include "GameExtraData.h"  // InventoryEntryData

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


	class Gem
	{
	public:
		Gem();
		bool empty();
		bool addGem(InventoryEntryData* a_entryData);
		InventoryEntryData* getEntryData();

	private:
		std::vector<InventoryEntryData*> _entryData;
	};


	class Soul
	{
	public:
		explicit Soul(UInt8 a_soulSize);
		bool empty();
		bool addSoul(InventoryEntryData* a_entryData);
		InventoryEntryData* findGem(bool a_partialFill);

	private:
		std::vector<Gem> _compatGems;
	};


	class SoulGem
	{
	public:
		SoulGem();
		bool empty();
		bool addSoulGem(InventoryEntryData* a_entryData, UInt8 a_soulSize);
		InventoryEntryData* findSoul(UInt8 a_soulSize, bool a_partialFill);

	private:
		std::vector<Soul> _souls;
	};


	bool isReusable(TESSoulGem* a_gem);
}