#include "SoulSeeker.h"

#include "GameAPI.h"  // g_thePlayer
#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // FormType
#include "GameReferences.h"  // TESObjectREFR
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry

#include "ExtraLocator.h"  // ExtraListLocator
#include "SoulSeekerLib.h"  // SoulGem, SoulSize
#include "RE_TESObjectREFR.h"  // RE::TESObjectREFR

#if _WIN64
#define CALL_MEMBER_FN_ENTRYDATA(entryData, fn) \
CALL_MEMBER_FN(entryData, fn)
#else
#include "RE_InventoryEntryData.h"
#define CALL_MEMBER_FN_ENTRYDATA(entryData, fn) \
CALL_MEMBER_FN(reinterpret_cast<RE::InventoryEntryData*>(entryData), fn)
#endif


namespace iEquip
{
	/**
	 * @brief Fetches the most optimal soulgem to fill an enchanted weapon.
	 * @param a_reqCharge The required soul to fetch. Valid inputs 1-5.
	 * @param a_fillMethod The method used to fill the soulgem. Valid inputs 0-1.
	 * @param a_partialFill Determines whether partially filled soul gems can be returned.
	 * @param a_wasteOK Determines whether soulgems exceeding the requred size can be returned.
	 * @return Returns the soul size of the found soul. Returns -1 if the search was a failure.
	 * @notes This function will remove the soulgem automatically.
	 **/
	SInt32 BringMeASoul(StaticFunctionTag* a_base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
	{
		if (!validateParams(a_reqCharge, a_fillMethod, a_partialFill)) {
			_ERROR("[ERROR] Parameters failed to validate!\n");
			return -1;
		}

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : 0;
		if (!containerData) {
			_ERROR("[ERROR] No container data!\n");
			return -1;
		}

		SoulGem candidates;
		if (!findCandidates(containerData, candidates)) {
			_ERROR("[ERROR] Failed to find candidates!\n");
			return -1;
		} else if (candidates.empty()) {
			_WARNING("[WARNING] No suitable soul gems!\n");
			return -1;
		}

		InventoryEntryData* optimalCandidate = findOptimalCandidate(candidates, a_reqCharge, a_fillMethod, a_partialFill, a_wasteOK);
		if (!optimalCandidate) {
			_ERROR("[ERROR] Failed to find an optimal candidate!\n");
			return -1;
		}

		int soulSize = CALL_MEMBER_FN_ENTRYDATA(optimalCandidate, GetSoulLevel)();
		if (optimalCandidate->extendDataList) {
			removeExtraSoul(containerData, optimalCandidate);
		}

		TESSoulGem* gem = static_cast<TESSoulGem*>(optimalCandidate->type);
		if (!isReusable(gem)) {
			TESObjectREFR* tmpObjRef = static_cast<TESObjectREFR*>(*g_thePlayer);
			RE::TESObjectREFR* objRef = reinterpret_cast<RE::TESObjectREFR*>(tmpObjRef);
			UInt32 droppedHandle;
			objRef->RemoveItem(&droppedHandle, gem, 1, RE::TESObjectREFR::kRemoveType_Remove, 0, 0, 0, 0);
		}
		return soulSize;
	}


	bool validateParams(UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill)
	{
		if (a_reqCharge < kSoulSize_Petty || a_reqCharge > kSoulSize_Grand) {
			_ERROR("[ERROR] Invalid soul size! (%i)\n", a_reqCharge);
			return false;
		} else if (!(a_fillMethod == kFillMethod_SmallerSoulsFirt || a_fillMethod == kFillMethod_UseLargestSoul)) {
			_ERROR("[ERROR] Invalid fill method! (%i)\n", a_fillMethod);
			return false;
		} else {
			return true;
		}
	}


	bool findCandidates(ExtraContainerChanges::Data* a_containerData, SoulGem& a_candidates)
	{
		InventoryEntryData* entryData = 0;
		UInt8 soulSize;
		for (UInt32 i = 0; i < a_containerData->objList->Count(); ++i) {
			entryData = a_containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formType == kFormType_SoulGem) {
					soulSize = CALL_MEMBER_FN_ENTRYDATA(entryData, GetSoulLevel)();
					if (soulSize > 0) {
						a_candidates.addSoulGem(entryData, soulSize) ? _DMESSAGE("[DEBUG] Successfully added soul gem\n") : _ERROR("[ERROR] Failed to add soul gem!\n");
					}
				}
			} else {
				_ERROR("[ERROR] No entry data found when attempting to find candidates!\n");
				return false;
			}
		}
		return true;
	}


	// Do NOT call this when there are no candidates!
	// Do NOT call this without validating the parameters!
	InventoryEntryData* findOptimalCandidate(SoulGem& a_candidates, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
	{
		InventoryEntryData* returnData = 0;
		UInt32 maxSoul = a_wasteOK ? kSoulSize_Grand : a_reqCharge;
		UInt32 loopCount = a_partialFill ? 2 : 1;
		switch (a_fillMethod) {
		case kFillMethod_SmallerSoulsFirt:
			while (loopCount--) {
				if (returnData = soulSearchUp(a_candidates, kSoulSize_Petty, maxSoul, a_partialFill)) {
					return returnData;
				}
				a_partialFill = true;
			}
			_DMESSAGE("[DEBUG] Search failed, loosening requirements\n");
			return soulSearchUp(a_candidates, kSoulSize_Petty, kSoulSize_Grand, true);
		case kFillMethod_UseLargestSoul:
			while (loopCount--) {
				if (returnData = soulSearchUp(a_candidates, a_reqCharge, maxSoul, a_partialFill)) {
					return returnData;
				}
				if (returnData = soulSearchDown(a_candidates, a_reqCharge - 1, kSoulSize_Petty, a_partialFill)) {
					return returnData;
				}
				a_partialFill = true;
			}
			_DMESSAGE("[DEBUG] Search failed, loosening requirements\n");
			return soulSearchDown(a_candidates, kSoulSize_Grand, kSoulSize_Petty, true);
		default:
			_ERROR("[ERROR] Invalid fill method! (%i)\n", a_fillMethod);
			return returnData;
		}
		_ERROR("[ERROR] A search failed to find a gem!\n");
		return returnData;
	}


	InventoryEntryData* soulSearchUp(SoulGem& a_candidates, UInt32 a_soulBegin, UInt32 a_soulEnd, bool a_partialFill)
	{
		InventoryEntryData* returnData = 0;
		for (UInt32 soulSize = a_soulBegin; soulSize <= a_soulEnd; ++soulSize) {
			if (returnData = a_candidates.findSoul(soulSize, a_partialFill)) {
				return returnData;
			}
		}
		return returnData;
	}


	InventoryEntryData* soulSearchDown(SoulGem& a_candidates, UInt32 a_soulBegin, UInt32 a_soulEnd, bool a_partialFill)
	{
		InventoryEntryData* returnData = 0;
		for (UInt32 soulSize = a_soulBegin; soulSize >= a_soulEnd; --soulSize) {
			if (returnData = a_candidates.findSoul(soulSize, a_partialFill)) {
				return returnData;
			}
		}
		return returnData;
	}


	void removeExtraSoul(ExtraContainerChanges::Data* a_containerData, InventoryEntryData* a_entry)
	{
		ExtraListLocator xListLocator(a_entry, { kExtraData_Soul }, { });
		BaseExtraList* xList = 0;
		BSExtraData* xData = 0;
		while (xList = xListLocator.found()) {
			xData = xList->m_data;
			while (xData) {
				if (xData->GetType() == kExtraData_Soul) {
					xList->Remove(kExtraData_Soul, xData);
				}
				xData = xData->next;
			}
		}
	}


	namespace SoulSeeker
	{
		bool RegisterFuncs(VMClassRegistry* a_registry)
		{
			a_registry->RegisterFunction(
				new NativeFunction4<StaticFunctionTag, SInt32, UInt32, UInt32, bool, bool>("BringMeASoul", "iEquip_SoulSeeker", BringMeASoul, a_registry));

			return true;
		}
	}
}
