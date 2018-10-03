#pragma once

#include "skse64/GameTypes.h"
#include "skse64/PapyrusNativeFunctions.h"
#include "skse64/PapyrusVM.h"
#include "skse64/PluginAPI.h"
#include "skse64/GameExtraData.h"
#include "skse64/GameForms.h"

#include <string>
#include <vector>


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


	enum FillMethod : UInt32
	{
		kSmallerSoulsFirt,
		kUseLargestSoul
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


#if 0
	struct SoulGem
	{
		SoulGem() : form(0), formID(0), soulLevel(0) {}
		SoulGem(TESForm* a_form, UInt32 a_formID, UInt32 a_soulLevel) : form(a_form), formID(a_formID), soulLevel(a_soulLevel) {}

		TESForm* form;
		UInt32 formID;
		UInt32 soulLevel;

		friend bool operator==(const SoulGem& lhs, const SoulGem& rhs) { return (lhs.formID == rhs.formID && lhs.soulLevel == rhs.soulLevel); }
		friend bool operator!=(const SoulGem& lhs, const SoulGem& rhs) { return !operator==(lhs, rhs); }
		friend bool operator< (const SoulGem& lhs, const SoulGem& rhs) { return lhs.formID == rhs.formID ? lhs.soulLevel < rhs.soulLevel : lhs.formID < rhs.formID; }
		friend bool operator> (const SoulGem& lhs, const SoulGem& rhs) { return  operator< (rhs, lhs); }
		friend bool operator<=(const SoulGem& lhs, const SoulGem& rhs) { return !operator> (lhs, rhs); }
		friend bool operator>=(const SoulGem& lhs, const SoulGem& rhs) { return !operator< (lhs, rhs); }

		friend bool operator< (const int& lhs, const SoulGem& rhs) { return }

		bool isPartiallyFilled()
		{
			switch (formID) {
			case kSoulGem_Petty:
				return soulLevel < kSoulSize_Petty;
			case kSoulGem_PettyFilled:
				return false;
			case kSoulGem_Lesser:
				return soulLevel < kSoulSize_Lesser;
			case kSoulGem_LesserFilled:
				return false;
			case kSoulGem_Common:
				return soulLevel < kSoulSize_Common;
			case kSoulGem_CommonFilled:
				return false;
			case kSoulGem_Greater:
				return soulLevel < kSoulSize_Greater;
			case kSoulGem_GreaterFilled:
				return false;
			case kSoulGem_Grand:
				return soulLevel < kSoulSize_Grand;
			case kSoulGem_GrandFilled:
				return false;
			case kSoulGem_Black:
				return soulLevel < kSoulSize_Grand;
			case kSoulGem_BlackFilled:
				return false;
			case kSoulGem_AzurasStar:
				return soulLevel < kSoulSize_Grand;
			case kSoulGem_BlackStar:
				return soulLevel < kSoulSize_Grand;
			default:
				return true;
			}
		}
	};
#endif


	UInt32 matchSoulToGem(UInt32 soulSize)
	{
		switch (soulSize) {
		case kSoulSize_Petty:
			return kSoulGem_PettyFilled;
		case kSoulSize_Lesser:
			return kSoulGem_LesserFilled;
		case kSoulSize_Common:
			return kSoulGem_CommonFilled;
		case kSoulSize_Greater:
			return kSoulGem_GreaterFilled;
		case kSoulSize_Grand:
			return kSoulGem_BlackStar;
		default:
			return 0;
		}
	}


	bool isValidSoulSize(UInt32 a_soulSize)
	{
		switch (a_soulSize) {
		case kSoulSize_Petty:
			return true;
		case kSoulSize_Lesser:
			return true;
		case kSoulSize_Common:
			return true;
		case kSoulSize_Greater:
			return true;
		case kSoulSize_Grand:
			return true;
		default:
			return false;
		}

	}


	bool isValidSoulGemType(UInt32 a_soulGemType)
	{
		switch (a_soulGemType) {
		case kSoulGem_Petty:
			return true;
		case kSoulGem_PettyFilled:
			return true;
		case kSoulGem_Lesser:
			return true;
		case kSoulGem_LesserFilled:
			return true;
		case kSoulGem_Common:
			return true;
		case kSoulGem_CommonFilled:
			return true;
		case kSoulGem_Greater:
			return true;
		case kSoulGem_GreaterFilled:
			return true;
		case kSoulGem_Grand:
			return true;
		case kSoulGem_GrandFilled:
			return true;
		case kSoulGem_Black:
			return true;
		case kSoulGem_BlackFilled:
			return true;
		case kSoulGem_AzurasStar:
			return true;
		case kSoulGem_BlackStar:
			return true;
		default:
			return false;
		}
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


	struct Gem
	{
		Gem(UInt32 a_soulGemType) : soulGemType(a_soulGemType) {}

		UInt32 soulGemType;
		std::vector<TESForm*> forms;

		bool addGem(TESForm* a_form)
		{
			forms.push_back(a_form);
		}

		bool empty()
		{
			return forms.empty();
		}
	};


	struct Soul
	{
		Soul(UInt32 a_soulSize) : soulSize(a_soulSize)
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

		UInt32 soulSize;
		std::vector<Gem> compatGems;

		bool addSoul(TESForm* a_form, UInt32 a_soulGemType)
		{
			for (auto gem : compatGems) {
				if (gem.soulGemType == a_soulGemType) {
					return gem.addGem(a_form);
				}
			}
			_ERROR("ERROR: In Soul::addSoul : Invalid soulgem type!");
		}

		bool empty()
		{
			for (auto gem : compatGems) {
				if (!gem.empty()) {
					return false;
				}
			}
			return true;
		}

		TESForm* findGem(bool a_partialFill)
		{
			if (empty()) {
				return 0;
			} else {
				UInt32 max = a_partialFill ? compatGems.size() : getMaxFillCount(soulSize);
				TESForm* form = 0;
				for (UInt32 i = 0; i < max; ++i) {
					if (!compatGems[i].empty()) {
						form = compatGems[i].forms.back();
						compatGems[i].forms.pop_back();
						return form;
					}
				}
			}
		}
	};


	struct SoulGem
	{
		std::vector<Soul> souls = { kSoulSize_Petty,
			                        kSoulSize_Lesser,
			                        kSoulSize_Common,
			                        kSoulSize_Greater,
			                        kSoulSize_Grand };

		bool addSoulGem(TESForm* a_form, UInt32 a_soulGemType, UInt32 a_soulSize)
		{
			for (auto soul : souls) {
				if (a_soulSize == soul.soulSize) {
					return soul.addSoul(a_form, a_soulGemType);
				}
			}
			_ERROR("ERROR: In SoulGem::addSoulGem : Invalid soul level!");
		}

		bool empty()
		{
			for (auto soul : souls) {
				if (!soul.empty()) {
					return false;
				}
			}
			return true;
		}

		TESForm* findSoul(UInt32 a_soulSize, bool a_partialFill)
		{
			return empty() ? 0 : souls[a_soulSize].findGem(a_partialFill);
		}
	};


	std::string IntToHexString(int a_val, int a_bytes);

	BSFixedString BringMeASoul(StaticFunctionTag* base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK);

	TESForm* iEquip_getForm(StaticFunctionTag* base);

	UInt32 iEquip_getSoulSize(StaticFunctionTag* base);

	bool findCandidates(ExtraContainerChanges::Data* containerData, std::vector<SoulGem>& candidates);

	SoulGem findOptimalCandidate(std::vector<SoulGem>& candidates, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK);

	void removeExtraSoul(ExtraContainerChanges::Data* containerData, UInt32 formID, UInt32 soulSize);

	bool isPlayerFilled(UInt32 formID);

	bool RegisterFuncs(VMClassRegistry* registry);


	static SoulGem optimalCandidate;
}