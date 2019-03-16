#pragma once

#include <vector>  // vector

#include "RE/BSTList.h"  // BSSimpleList

class InventoryEntryData;
class TESSoulGem;
class VMClassRegistry;
struct StaticFunctionTag;


namespace
{
	enum
	{
		kReusableSoulGem = 0x000ED2F1
	};


	enum class FillMethod : UInt32
	{
		kSmallerSoulsFirst = 0,
		kUseLargestSoul = 1
	};


	enum class SoulLevel : SInt8
	{
		kNone = 0,
		kPetty = 1,
		kLesser = 2,
		kCommon = 3,
		kGreater = 4,
		kGrand = 5
	};


	struct SoulGem
	{
		constexpr SoulGem() :
			gemSize(SoulLevel::kNone),
			soulSize(SoulLevel::kNone),
			origSoulSize(SoulLevel::kNone),
			entryData(0)
		{}


		constexpr SoulGem(SoulLevel a_gemSize, SoulLevel a_soulSize, InventoryEntryData* a_entryData) :
			gemSize(a_gemSize),
			soulSize(a_soulSize),
			origSoulSize(a_soulSize),
			entryData(a_entryData)
		{}


		SoulLevel			gemSize;
		SoulLevel			soulSize;
		SoulLevel			origSoulSize;
		InventoryEntryData*	entryData;
	};


	using GemList = std::vector<SoulGem>;


	const SoulGem&	NearestNeighbour(const GemList& a_gems, const SoulGem& a_comp);
	bool			ValidateParams(SoulLevel a_reqCharge, FillMethod a_fillMethod);
	void			RemoveExtraSoul(InventoryEntryData* a_entry);
	bool			IsReusable(TESSoulGem* a_gem);
	void			ApplyVerticalShift(SoulLevel a_reqCharge, FillMethod a_fillMethod, GemList& a_gems);
	GemList			ParseInventory(RE::BSSimpleList<InventoryEntryData*>& a_entryList, bool a_partialFill);
}


SInt32 BringMeASoul(StaticFunctionTag* a_base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK);


namespace SoulSeeker
{
	bool RegisterFuncs(VMClassRegistry* a_registry);
}
