#include "SoulSeeker.h"

#include "GameAPI.h"  // g_thePlayer
#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // ExtraContainerChanges, InventoryEntryData
#include "GameForms.h"  // FormType
#include "GameObjects.h"  // TESSoulGem
#include "GameReferences.h"  // TESObjectREFR
#include "PapyrusNativeFunctions.h"  // StaticFunctionTag
#include "PapyrusVM.h"  // VMClassRegistry

#include <cmath>  // powf, sqrtf
#include <cstdlib>  // size_t
#include <limits>  // numeric_limits

#include "RE/BSTList.h"  // BSSimpleList
#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR


#if _WIN64
#define CALL_MEMBER_FN_ENTRYDATA(entryData, fn) \
CALL_MEMBER_FN(entryData, fn)
#else
#include "RE_InventoryEntryData.h"
#define CALL_MEMBER_FN_ENTRYDATA(entryData, fn) \
CALL_MEMBER_FN(reinterpret_cast<RE::InventoryEntryData*>(entryData), fn)
#endif


namespace
{
	const SoulGem& NearestNeighbour(const GemList& a_gems, const SoulGem& a_comp)
	{
		static auto diffX = [](const SoulGem& a_lhs, const SoulGem& a_rhs) -> float
		{
			return static_cast<float>(a_lhs.gemSize - a_rhs.gemSize);
		};

		static auto diffY = [](const SoulGem& a_lhs, const SoulGem& a_rhs)-> float
		{
			return static_cast<float>(a_lhs.soulSize - a_rhs.soulSize);
		};

		static auto euclidean_distance = [](const SoulGem& a_lhs, const SoulGem& a_rhs) -> float
		{
			return std::sqrtf(std::powf(diffX(a_lhs, a_rhs), 2) + std::powf(diffY(a_lhs, a_rhs), 2));
		};

		std::size_t idx = 0;
		float min = std::numeric_limits<float>::max();
		float tmp;
		for (std::size_t i = 0; i < a_gems.size(); ++i) {
			tmp = euclidean_distance(a_gems[i], a_comp);
			if (tmp < min) {
				min = tmp;
				idx = i;
			}
		}
		return a_gems[idx];
	}


	bool ValidateParams(SoulLevel a_reqCharge, FillMethod a_fillMethod)
	{
		if (a_reqCharge < SoulLevel::kPetty || a_reqCharge > SoulLevel::kGrand) {
			_ERROR("[ERROR] Invalid soul size! (%i)\n", to_underlying(a_reqCharge));
			return false;
		}

		switch (a_fillMethod) {
		case FillMethod::kSmallerSoulsFirst:
		case FillMethod::kUseLargestSoul:
			break;
		default:
			_ERROR("[ERROR] Invalid fill method! (%i)\n", to_underlying(a_fillMethod));
			return false;
		}

		return true;
	}


	void RemoveExtraSoul(InventoryEntryData* a_entry)
	{
		if (a_entry->extendDataList) {
			RE::BSSimpleList<BaseExtraList*>* extendDataList = (RE::BSSimpleList<BaseExtraList*>*)a_entry->extendDataList;
			ExtraSoul* xSoul = 0;
			for (auto& xList : *extendDataList) {
				xSoul = static_cast<ExtraSoul*>(xList->GetByType(kExtraData_Soul));
				if (xSoul) {
					xList->Remove(kExtraData_Soul, xSoul);
					break;
				}
			}
		}
	}


	bool IsReusable(TESSoulGem* a_gem)
	{
		BGSKeyword* ReusableSoulGem = static_cast<BGSKeyword*>(LookupFormByID(kReusableSoulGem));
		return a_gem->keyword.HasKeyword(ReusableSoulGem);
	}


	void ApplyVerticalShift(SoulLevel a_reqCharge, FillMethod a_fillMethod, GemList& a_gems)
	{
		switch (a_fillMethod) {
		case FillMethod::kSmallerSoulsFirst:
			for (auto& gem : a_gems) {
				gem.soulSize += a_reqCharge;
			}
			break;
		case FillMethod::kUseLargestSoul:
			for (auto& gem : a_gems) {
				if (gem.soulSize < a_reqCharge) {
					gem.soulSize = a_reqCharge - gem.soulSize + SoulLevel::kGrand;
				}
			}
			break;
		}
	}


	GemList ParseInventory(RE::BSSimpleList<InventoryEntryData*>& a_entryList, bool a_partialFill)
	{
		GemList acceptedGems;
		GemList rejectedGems;
		for (auto& entry : a_entryList) {
			if (entry->type && entry->type->formType == kFormType_SoulGem) {
				TESSoulGem* soulGem = static_cast<TESSoulGem*>(entry->type);
				SoulLevel gemSize = static_cast<SoulLevel>(soulGem->gemSize);
				SoulLevel soulSize = static_cast<SoulLevel>(CALL_MEMBER_FN_ENTRYDATA(entry, GetSoulLevel)());
				if (soulSize > SoulLevel::kNone) {
					SoulGem gem(gemSize, soulSize, entry);
					if (!a_partialFill) {
						if (gem.soulSize == gem.gemSize) {
							acceptedGems.push_back(gem);
						} else {
							rejectedGems.push_back(gem);
						}
					} else {
						acceptedGems.push_back(gem);
					}
				}
			}
		}
		return acceptedGems.empty() ? rejectedGems : acceptedGems;
	}
}


SInt32 BringMeASoul(StaticFunctionTag* a_base, UInt32 a_reqCharge, UInt32 a_fillMethod, bool a_partialFill, bool a_wasteOK)
{
	using RemoveType = RE::TESObjectREFR::RemoveType;

	SoulLevel reqCharge = static_cast<SoulLevel>(a_reqCharge);
	FillMethod fillMethod = static_cast<FillMethod>(a_fillMethod);
	if (!ValidateParams(reqCharge, fillMethod)) {
		_ERROR("[ERROR] Parameters failed to validate!\n");
		return -1;
	}

	ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
	auto invChanges = containerChanges ? containerChanges->data : 0;
	if (!invChanges) {
		_ERROR("[ERROR] No inventory cahnges!\n");
		return -1;
	}
	RE::BSSimpleList<InventoryEntryData*>* entryList = reinterpret_cast<RE::BSSimpleList<InventoryEntryData*>*>(invChanges->objList);
	if (!entryList) {
		_ERROR("[ERROR] No entry list!\n");
		return -1;
	}

	GemList gems = ParseInventory(*entryList, a_partialFill);
	if (gems.empty()) {
		_ERROR("[ERROR] Found no soul gems!\n");
		return -1;
	}

	ApplyVerticalShift(reqCharge, fillMethod, gems);

	SoulGem idealGem(reqCharge, reqCharge, 0);
	SoulGem foundGem;
	if (!a_wasteOK) {
		GemList noWaste;
		for (auto& gem : gems) {
			if (gem.soulSize <= reqCharge) {
				noWaste.push_back(gem);
			}
		}
		if (!noWaste.empty()) {
			foundGem = NearestNeighbour(noWaste, idealGem);
		}
	}

	if (!foundGem.entryData) {
		foundGem = NearestNeighbour(gems, idealGem);
	}

	RemoveExtraSoul(foundGem.entryData);

	TESSoulGem* gem = static_cast<TESSoulGem*>(foundGem.entryData->type);
	if (!IsReusable(gem)) {
		RE::TESObjectREFR* objRef = reinterpret_cast<RE::TESObjectREFR*>(*g_thePlayer);
		UInt32 droppedHandle;
		objRef->RemoveItem(droppedHandle, gem, 1, RemoveType::kRemove, 0, 0);
	}
	return static_cast<UInt32>(foundGem.origSoulSize);
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
