#pragma once

#include "GameExtraData.h"  // ExtraContainerChanges::Data

#include "SoulSeekerLib.h"  // SoulGem

class InventoryEntryData;
class VMClassRegistry;
struct StaticFunctionTag;


namespace iEquip
{
	class SoulGem;


	enum FillMethod : UInt32
	{
		kFillMethod_SmallerSoulsFirt,
		kFillMethod_UseLargestSoul
	};


	SInt32 BringMeASoul(StaticFunctionTag* a_base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK);
	bool validateParams(UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill);
	bool findCandidates(ExtraContainerChanges::Data* a_containerData, SoulGem& a_candidates);
	InventoryEntryData* findOptimalCandidate(SoulGem& a_candidates, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK);
	InventoryEntryData* soulSearchUp(SoulGem& a_candidates, UInt32 a_soulBegin, UInt32 a_soulEnd, bool a_partialFill);
	InventoryEntryData* soulSearchDown(SoulGem& a_candidates, UInt32 a_soulBegin, UInt32 a_soulEnd, bool a_partialFill);
	void removeExtraSoul(ExtraContainerChanges::Data* a_containerData, InventoryEntryData* a_entry);


	namespace SoulSeeker
	{
		bool RegisterFuncs(VMClassRegistry* a_registry);
	}
}
