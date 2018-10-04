#include "iEquip_SoulSeeker.h"

#include "common/IDebugLog.h"  // gLog
#include "common/ITypes.h"  // UInt32
#include "skse64/GameAPI.h"  // g_thePlayer
#include "skse64/GameBSExtraData.h"  // ExtraDataType, BaseExtraList
#include "skse64/GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "skse64/GameForms.h"  // TESForm, FormType
#include "skse64/PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "skse64/PapyrusVM.h"  // VMClassRegistry

#include <string>  // string

#include "SoulGem.h"  // SoulGem, SoulSize, SoulGemType
#include "Utility.h"  // boolToString


namespace iEquip_SoulSeeker
{
	/**
	 * @brief Fetch the most optimal soulgem to fill an enchanted weapon
	 * @param a_reqCharge The required soul to fetch. Valid inputs are defined in SoulSize.
	 * @param a_fillMethod The method used to fill the soulgem. Valid inputs are defined in FillMethod.
	 * @param a_partialFill Determines whether partially filled soul gems can be returned.
	 * @param a_wasteOK Determines whether soulgems exceeding the requred size can be returned.
	 * @return Returns an empty string if successful, otherwise returns an exception.
	 **/
	TESForm* BringMeASoul(StaticFunctionTag* a_base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
	{
		// TODO: Remove dbg info
		{
			std::string msg;
			_DMESSAGE("");
			msg = "a_reqCharge == " + std::to_string(a_reqCharge);
			_DMESSAGE(msg.c_str());
			msg = "a_fillMethod == " + std::to_string(a_fillMethod);
			_DMESSAGE(msg.c_str());
			msg = "a_partialFill == " + boolToString(a_partialFill);
			_DMESSAGE(msg.c_str());
			msg = "a_wasteOK == " + boolToString(a_wasteOK);
			_DMESSAGE(msg.c_str());
			_DMESSAGE("");
		}
		// TODO: Remove dbg info

		if (!validateParams(a_reqCharge, a_fillMethod, a_partialFill)) {
			_ERROR("ERROR: Parameters failed to validate!");
			return 0;
		}

		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : NULL;
		if (!containerData) {
			_ERROR("ERROR: No container data!");
			return 0;
		}

		SoulGem candidates;
		if (!findCandidates(containerData, candidates)) {
			_ERROR("ERROR: Failed to find candidates!");
			return 0;
		} else if (candidates.empty()) {
			_WARNING("WARNING: No suitable soul gems!");
			return 0;
		}

		InventoryEntryData* optimalCandidate = findOptimalCandidate(candidates, a_reqCharge, a_fillMethod, a_partialFill, a_wasteOK);
		if (!optimalCandidate) {
			_ERROR("ERROR: Failed to find an optimal candidate!");
			return 0;
		}

		if (isPlayerFilled(optimalCandidate->type->formID)) {
			removeExtraSoul(containerData, optimalCandidate);
		}

		return optimalCandidate->type;
	}


	bool validateParams(UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill)
	{
		if (a_reqCharge < kSoulSize_Petty || a_reqCharge > kSoulSize_Grand) {
			std::string msg = "ERROR: Invalid soul size! (" + std::to_string(a_reqCharge) + ")";
			_ERROR(msg.c_str());
			return false;
		} else if (!(a_fillMethod == kFillMethod_SmallerSoulsFirt || a_fillMethod == kFillMethod_UseLargestSoul)) {
			std::string msg = "ERROR: Invalid fill method! (" + std::to_string(a_fillMethod) + ")";
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
					soulSize = CALL_MEMBER_FN(entryData, GetSoulLevel)();
					if (soulSize > 0) {
						a_candidates.addSoulGem(entryData, soulSize) ? _DMESSAGE("Successfully added soul gem") : _ERROR("ERROR: Failed to add soul gem!");
					}
				}
			} else {
				_ERROR("ERROR: No entry data found when attempting to find candidates!");
				return false;
			}
		}
		return true;
	}


	// Do NOT call this when there are no candidates!
	// Do NOT call this without validating the parameters!
	InventoryEntryData* findOptimalCandidate(SoulGem& a_candidates, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
	{
		InventoryEntryData* returnData;
		switch (a_fillMethod) {
		case kFillMethod_SmallerSoulsFirt:
			for (UInt32 soulSize = kSoulSize_Petty; soulSize <= a_reqCharge; ++soulSize) {
				if (returnData = a_candidates.findSoul(soulSize, a_partialFill)) {
					return returnData;
				}
			}
			// Search failed, loosening requirements
			return findOptimalCandidate(a_candidates, kSoulSize_Grand, kFillMethod_SmallerSoulsFirt, true, true);
		case kFillMethod_UseLargestSoul:
			for (UInt32 soulSize = a_reqCharge; soulSize >= kSoulSize_Petty; --soulSize) {
				if (returnData = a_candidates.findSoul(soulSize, a_partialFill)) {
					return returnData;
				}
			}
			// Search failed, loosening requirements
			for (UInt32 soulSize = a_reqCharge + 1; soulSize <= kSoulSize_Grand; ++soulSize) {
				if (returnData = a_candidates.findSoul(soulSize, true)) {
					return returnData;
				}
			}
		default:
			std::string msg = "ERROR: Invalid fill method! (" + std::to_string(a_fillMethod) + ")";
			_ERROR(msg.c_str());
			return 0;
		}
		_ERROR("ERROR: A search failed to find a gem!");
		return 0;
	}


	// Do NOT call this if the soul gem is a prefilled type!
	void removeExtraSoul(ExtraContainerChanges::Data* a_containerData, InventoryEntryData* a_entry)
	{
		InventoryEntryData* entryData = 0;
		BaseExtraList* extraList = 0;
		for (UInt32 i = 0; i < a_containerData->objList->Count(); ++i) {
			entryData = a_containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formType == kFormType_SoulGem &&
					entryData->type->formID == a_entry->type->formID &&
					CALL_MEMBER_FN(entryData, GetSoulLevel)() == CALL_MEMBER_FN(a_entry, GetSoulLevel)()) {
					extraList = entryData->extendDataList->GetNthItem(0);
					extraList->Remove(kExtraData_Soul, extraList->m_data);
				}
			} else {
				_ERROR("ERROR: No entry data found when attempting to remove extra soul!");
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


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction4<StaticFunctionTag, TESForm*, UInt32, UInt32, bool, bool>("BringMeASoul", "iEquip_SoulSeeker", iEquip_SoulSeeker::BringMeASoul, a_registry));

		return true;
	}
}