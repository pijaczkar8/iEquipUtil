#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "skse64/GameForms.h"  // TESForm
#include "skse64/PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "skse64/PapyrusVM.h"  // VMClassRegistry

#include "SoulGem.h"  // SoulGem


namespace iEquip_SoulSeeker
{
	enum FillMethod : UInt32
	{
		kFillMethod_SmallerSoulsFirt,
		kFillMethod_UseLargestSoul
	};


	static UInt32 lastFoundSoulSize = 0;
	static InventoryEntryData* optimalCandidate = 0;


	TESForm* BringMeASoul(StaticFunctionTag* a_base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK);
	UInt32 GetSoulSize(StaticFunctionTag* a_base);
	bool validateParams(UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill);
	bool findCandidates(ExtraContainerChanges::Data* a_containerData, SoulGem& a_candidates);
	InventoryEntryData* findOptimalCandidate(SoulGem& a_candidates, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK);
	void removeExtraSoul(ExtraContainerChanges::Data* a_containerData, InventoryEntryData* a_entry);
	bool isPlayerFilled(UInt32 a_formID);
	bool isReusable(UInt32 a_formID);
	bool RegisterFuncs(VMClassRegistry* a_registry);
}