#include "iEquip_SoulSeeker.h"

#include "IDebugLog.h"  // gLog
#include "ITypes.h"  // UInt32
#include "GameAPI.h"  // g_thePlayer
#include "GameBSExtraData.h"  // ExtraDataType, BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // TESForm, FormType
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry
#include "Utilities.h"  // CALL_MEMBER_FN

#include <string>  // string

#include "iEquip_SoulSeekerLib.h"  // SoulGem, SoulSize, SoulGemType
#include "iEquip_Utility.h"  // boolToString


#if _WIN64
#define CALL_MEMBER_FN_ENTRYDATA(entryData, fn) \
CALL_MEMBER_FN(entryData, fn)
#else
#include "RE_InventoryEntryData.h"
#define CALL_MEMBER_FN_ENTRYDATA(entryData, fn) \
CALL_MEMBER_FN(reinterpret_cast<RE::InventoryEntryData*>(entryData), fn)
#endif


namespace iEquip_SoulSeeker
{
	/**
	 * @brief Fetch the most optimal soulgem to fill an enchanted weapon
	 * @param a_reqCharge The required soul to fetch. Valid inputs 1-5.
	 * @param a_fillMethod The method used to fill the soulgem. Valid inputs 0-1.
	 * @param a_partialFill Determines whether partially filled soul gems can be returned.
	 * @param a_wasteOK Determines whether soulgems exceeding the requred size can be returned.
	 * @return Returns the Form to be removed. Call GetSoulSize to determine if the search was a success.
	 **/
	TESForm* BringMeASoul(StaticFunctionTag* a_base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
	{
		{
			std::string msg;
			msg = "a_reqCharge == " + std::to_string(a_reqCharge);
			_DMESSAGE(msg.c_str());
			msg = "a_fillMethod == " + std::to_string(a_fillMethod);
			_DMESSAGE(msg.c_str());
			msg = "a_partialFill == " + iEquip_Utility::boolToString(a_partialFill);
			_DMESSAGE(msg.c_str());
			msg = "a_wasteOK == " + iEquip_Utility::boolToString(a_wasteOK);
			_DMESSAGE(msg.c_str());
			_DMESSAGE("");
		}

		lastFoundSoulSize = 0;
		optimalCandidate = 0;

		if (!validateParams(a_reqCharge, a_fillMethod, a_partialFill)) {
			_ERROR("ERROR: Parameters failed to validate!\n");
			return 0;
		}

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : 0;
		if (!containerData) {
			_ERROR("ERROR: No container data!\n");
			return 0;
		}

		SoulGem candidates;
		if (!findCandidates(containerData, candidates)) {
			_ERROR("ERROR: Failed to find candidates!\n");
			return 0;
		} else if (candidates.empty()) {
			_WARNING("WARNING: No suitable soul gems!\n");
			return 0;
		}

		optimalCandidate = findOptimalCandidate(candidates, a_reqCharge, a_fillMethod, a_partialFill, a_wasteOK);
		if (!optimalCandidate) {
			_ERROR("ERROR: Failed to find an optimal candidate!\n");
			return 0;
		}

		if (isPlayerFilled(optimalCandidate->type->formID)) {
			removeExtraSoul(containerData, optimalCandidate);
		}

		lastFoundSoulSize = CALL_MEMBER_FN_ENTRYDATA(optimalCandidate, GetSoulLevel)();
		return isReusable(optimalCandidate->type->formID) ? 0 : optimalCandidate->type;
	}


	/**
	 * @brief Gets the soul size of the last returned soul.
	 * @return Returns the size of the last returned soul if the search was a success, otherwise returns 0.
	 **/
	UInt32 GetSoulSize(StaticFunctionTag* a_base)
	{
		return lastFoundSoulSize;
	}


	bool validateParams(UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill)
	{
		if (a_reqCharge < kSoulSize_Petty || a_reqCharge > kSoulSize_Grand) {
			std::string msg = "ERROR: Invalid soul size! (" + std::to_string(a_reqCharge) + ")\n";
			_ERROR(msg.c_str());
			return false;
		} else if (!(a_fillMethod == kFillMethod_SmallerSoulsFirt || a_fillMethod == kFillMethod_UseLargestSoul)) {
			std::string msg = "ERROR: Invalid fill method! (" + std::to_string(a_fillMethod) + ")\n";
			_ERROR(msg.c_str());
			return false;
		} else {
			return true;
		}
	}


	bool findCandidates(ExtraContainerChanges::Data* a_containerData, SoulGem& a_candidates)
	{
		UInt32 soulSize;
		InventoryEntryData* entryData = 0;
		for (UInt32 i = 0; i < a_containerData->objList->Count(); ++i) {
			entryData = a_containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formType == kFormType_SoulGem) {
					soulSize = CALL_MEMBER_FN_ENTRYDATA(entryData, GetSoulLevel)();
					if (soulSize > 0) {
						a_candidates.addSoulGem(entryData, soulSize) ? _DMESSAGE("Successfully added soul gem\n") : _ERROR("ERROR: Failed to add soul gem!");
					}
				}
			} else {
				_ERROR("ERROR: No entry data found when attempting to find candidates!\n");
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
			_DMESSAGE("Search failed, loosening requirements\n");
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
			_DMESSAGE("Search failed, loosening requirements\n");
			return soulSearchDown(a_candidates, kSoulSize_Grand, kSoulSize_Petty, true);
		default:
			std::string msg = "ERROR: Invalid fill method! (" + std::to_string(a_fillMethod) + ")\n";
			_ERROR(msg.c_str());
			return returnData;
		}
		_ERROR("ERROR: A search failed to find a gem!\n");
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


	// Do NOT call this if the soul gem is a prefilled type!
	void removeExtraSoul(ExtraContainerChanges::Data* a_containerData, InventoryEntryData* a_entry)
	{
		InventoryEntryData* entryData = 0;
		BaseExtraList* extraList = 0;
		UInt32 soulLevel = CALL_MEMBER_FN_ENTRYDATA(a_entry, GetSoulLevel)();
		for (UInt32 i = 0; i < a_containerData->objList->Count(); ++i) {
			entryData = a_containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formType == kFormType_SoulGem &&
					entryData->type->formID == a_entry->type->formID &&
					CALL_MEMBER_FN_ENTRYDATA(entryData, GetSoulLevel)() == soulLevel) {
					extraList = entryData->extendDataList->GetNthItem(0);
					extraList->Remove(kExtraData_Soul, extraList->m_data);
				}
			} else {
				_ERROR("ERROR: No entry data found when attempting to remove extra soul!\n");
			}
		}
	}


	bool isPlayerFilled(UInt32 a_formID)
	{
		switch (a_formID) {
		case kSoulGem_Petty:
			return true;
		case kSoulGem_PettyFilled:
			return false;
		case kSoulGem_Lesser:
			return true;
		case kSoulGem_LesserFilled:
			return false;
		case kSoulGem_Common:
			return true;
		case kSoulGem_CommonFilled:
			return false;
		case kSoulGem_Greater:
			return true;
		case kSoulGem_GreaterFilled:
			return false;
		case kSoulGem_Grand:
			return true;
		case kSoulGem_GrandFilled:
			return false;
		case kSoulGem_Black:
			return true;
		case kSoulGem_BlackFilled:
			return false;
		case kSoulGem_AzurasStar:
			return true;
		case kSoulGem_BlackStar:
			return true;
		default:
			return false;
		}
	}


	bool isReusable(UInt32 a_formID)
	{
		switch (a_formID) {
		case kSoulGem_AzurasStar:
			return true;
		case kSoulGem_BlackStar:
			return true;
		default:
			return false;
		}
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction4<StaticFunctionTag, TESForm*, UInt32, UInt32, bool, bool>("BringMeASoul", "iEquip_SoulSeeker", iEquip_SoulSeeker::BringMeASoul, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction0<StaticFunctionTag, UInt32>("GetSoulSize", "iEquip_SoulSeeker", iEquip_SoulSeeker::GetSoulSize, a_registry));

		return true;
	}
}