#include "iEquip_SoulSeeker.h"

#include "skse64/GameTypes.h"
#include "skse64/PapyrusNativeFunctions.h"
#include "skse64/PapyrusVM.h"
#include "skse64/PluginAPI.h"
#include "skse64/GameReferences.h"
#include "skse64/GameExtraData.h"
#include "skse64/GameForms.h"
#include "skse64/GameBSExtraData.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>


namespace iEquip_SoulSeeker
{
	std::string IntToHexString(int a_val, int a_bytes)
	{
		// Convert to hex
		std::stringstream sstream;
		sstream << std::hex << a_val;
		std::string str = sstream.str();

		// Convert lowercase characters to uppercase
		for (auto & c : str) {
			c = toupper(c);
		}

		// Sign extension
		while (std::strlen(str.c_str()) < a_bytes * 2) {
			str = '0' + str;
		}

		return str;
	}


	/**
	 * @brief Fetch the most optimal soulgem to fill an enchanted weapon
	 * @param a_reqCharge The required soul to fetch. Valid inputs are defined in SoulSize.
	 * @param a_fillMethod The method used to fill the soulgem. Valid inputs are defined in FillMethod.
	 * @param a_partialFill Determines whether partially filled soul gems can be returned.
	 * @param a_wasteOK Determines whether soulgems exceeding the requred size can be returned.
	 * @return Returns an empty string if successful, otherwise returns an exception.
	 **/
	BSFixedString BringMeASoul(StaticFunctionTag* base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
	{
		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
		ExtraContainerChanges::Data* containerData = containerChanges ? containerChanges->data : NULL;
		if (!containerData) {
			_ERROR("ERROR: No container data!");
			return "exceptionNoContainerData";
		}

		std::vector<SoulGem> candidates;
		if (!findCandidates(containerData, candidates)) {
			_ERROR("ERROR: No entry data!");
			return "exceptionNoEntryData";
		} else if (candidates.empty()) {
			_ERROR("ERROR: No suitable soul gems!");
			return "exceptionNoSuitableSoulGems";
		}

		optimalCandidate = findOptimalCandidate(candidates, a_reqCharge, a_fillMethod, a_partialFill, a_wasteOK);

		removeExtraSoul(containerData, optimalCandidate.formID, optimalCandidate.soulLevel);

		return "";
	}


	TESForm* iEquip_getForm(StaticFunctionTag* base)
	{
		return optimalCandidate.form;
	}


	UInt32 iEquip_getSoulLevel(StaticFunctionTag* base)
	{
		return optimalCandidate.soulLevel;
	}


	bool findCandidates(ExtraContainerChanges::Data* containerData, SoulGem& candidates)
	{
		UInt32 soulSize;
		InventoryEntryData* entryData = 0;
		for (UInt32 i = 0; i < containerData->objList->Count(); ++i) {
			entryData = containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formType == kFormType_SoulGem) {
					soulSize = CALL_MEMBER_FN(entryData, GetSoulLevel)();
					if (soulSize > 0) {
						candidates.addSoulGem(entryData->type, entryData->type->formID, soulSize);
					}
				}
			} else {
				_ERROR("ERROR: No entry data!");
				return false;
			}
		}
		return true;
	}


	// TODO: Add function to find first soul in list for exception case
	// DO NOT call this when there are no candidates!
	TESForm* findOptimalCandidate(SoulGem& candidates, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
	{
		TESForm* returnForm;
		switch (a_fillMethod) {
		case kSmallerSoulsFirt:
			for (UInt32 soulSize = kSoulSize_Petty; soulSize <= a_reqCharge; ++soulSize) {
				if (returnForm = candidates.findSoul(soulSize, a_partialFill)) {
					return returnForm;
				}
			}
			// Search failed, loosening requirements
			return findOptimalCandidate(candidates, kSoulSize_Grand, kSmallerSoulsFirt, true, true);
		case kUseLargestSoul:
			for (UInt32 soulSize = a_reqCharge; soulSize >= kSoulSize_Petty; --soulSize) {
				if (returnForm = candidates.findSoul(soulSize, a_partialFill)) {
					return returnForm;
				}
			}
			// Search failed, loosening requirements
			return findOptimalCandidate(candidates, kSoulSize_Grand, kUseLargestSoul, true, true);
		default:
			return 0;
		}
	}


	void removeExtraSoul(ExtraContainerChanges::Data* containerData, UInt32 formID, UInt32 soulSize)
	{
		InventoryEntryData* entryData = 0;
		BaseExtraList* extraList = 0;
		for (UInt32 i = 0; i < containerData->objList->Count(); ++i) {
			entryData = containerData->objList->GetNthItem(i);
			if (entryData) {
				if (entryData->type->formType == kFormType_SoulGem &&
					entryData->type->formID == formID &&
					CALL_MEMBER_FN(entryData, GetSoulLevel)() == soulSize) {
					if (isPlayerFilled(formID)) {
						extraList = entryData->extendDataList->GetNthItem(0);
						extraList->Remove(kExtraData_Soul, extraList->m_data);
					}

				}
			} else {
				_ERROR("ERROR: No entry data!");
			}
		}
	}


	bool isPlayerFilled(UInt32 formID)
	{
		switch (formID) {
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
		}
	}


	bool RegisterFuncs(VMClassRegistry* registry)
	{
		registry->RegisterFunction(
			new NativeFunction4<StaticFunctionTag, BSFixedString, UInt32, UInt32, bool, bool>("BringMeASoul", "iEquip_SoulSeeker", iEquip_SoulSeeker::BringMeASoul, registry));

		registry->RegisterFunction(
			new NativeFunction0<StaticFunctionTag, TESForm*>("iEquip_getForm", "iEquip_SoulSeeker", iEquip_SoulSeeker::iEquip_getForm, registry));

		registry->RegisterFunction(
			new NativeFunction0<StaticFunctionTag, UInt32>("iEquip_getSoulSize", "iEquip_SoulSeeker", iEquip_SoulSeeker::iEquip_getSoulSize, registry));

		return true;
	}
}