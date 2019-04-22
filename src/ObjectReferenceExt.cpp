#include "ObjectReferenceExt.h"

#include "GameObjects.h"  // TESForm
#include "GameReferences.h"  // TESObjectREFR
#include "GameRTTI.h"  // DYNAMIC_CAST
#include "PapyrusNativeFunctions.h"  // NativeFunction
#include "PapyrusVM.h"  // VMClassRegistry

#include <map>  // map
#include <set>  // set

#include "InventoryUtil.h"  // ForEachInvEntry


namespace
{
	enum
	{
		kFormID_Gold = 0x0000000F
	};
}


namespace ObjectReferenceExt
{
	SInt32 GetNumItemsOfType(StaticFunctionTag*, TESObjectREFR* a_container, UInt32 a_type)
	{
		if (!a_container) {
			_WARNING("[WARNING] a_container is a NONE form!");
			return -1;
		} else if (a_type > kFormType_Max) {
			_WARNING("[WARNING] a_type is out of range!");
			return -1;
		}

		auto container = DYNAMIC_CAST(a_container->baseForm, TESForm, TESContainer);
		if (!container) {
			_WARNING("[WARNING] a_container does not reference a container!");
			return -1;
		}

		// extra items
		using FormID = UInt32;
		using Count = SInt32;
		std::map<FormID, Count> itemMap;
		ForEachInvEntry(a_container, [&](InventoryEntryData* a_entryData) -> bool
		{
			if (a_entryData->type->formType == a_type) {
				itemMap.insert({ a_entryData->type->formID, a_entryData->countDelta });
			}
			return true;
		});

		// default inv
		for (UInt32 i = 0; i < container->numEntries; ++i) {
			auto entry = container->entries[i];
			if (entry->form->formType == a_type) {
				auto result = itemMap.insert({ entry->form->formID, entry->count });
				if (!result.second && entry->form->formID != kFormID_Gold) {
					result.first->second += entry->count;
				}
			}
		}

		SInt32 count = 0;
		for (auto& item : itemMap) {
			if (item.second > 0) {
				++count;
			}
		}
		return count;
	}


	TESForm* GetNthFormOfType(StaticFunctionTag*, TESObjectREFR* a_container, UInt32 a_type, UInt32 a_n)
	{
		if (!a_container) {
			_WARNING("[WARNING] a_container is a NONE form!");
			return 0;
		} else if (a_type > kFormType_Max) {
			_WARNING("[WARNING] a_type is out of range!");
			return 0;
		}

		auto container = DYNAMIC_CAST(a_container->baseForm, TESForm, TESContainer);
		if (!container) {
			_WARNING("[WARNING] a_container does not reference a container!");
			return 0;
		}

		// extra items
		using FormID = UInt32;
		using Count = SInt32;
		std::map<FormID, Count> itemMap;
		ForEachInvEntry(a_container, [&](InventoryEntryData* a_entryData) -> bool
		{
			if (a_entryData->type->formType == a_type) {
				itemMap.insert({ a_entryData->type->formID, a_entryData->countDelta });
			}
			return true;
		});

		// default inv
		for (UInt32 i = 0; i < container->numEntries; ++i) {
			auto entry = container->entries[i];
			if (entry->form->formType == a_type) {
				auto result = itemMap.insert({ entry->form->formID, entry->count });
				if (!result.second && entry->form->formID != kFormID_Gold) {
					result.first->second += entry->count;
				}
			}
		}

		for (auto& item : itemMap) {
			if (item.second > 0 && a_n-- == 0) {
				return LookupFormByID(item.first);
			}
		}
		return 0;
	}


	bool RegisterFuncs(VMClassRegistry* a_registry)
	{
		a_registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, SInt32, TESObjectREFR*, UInt32>("GetNumItemsOfType", "iEquip_ObjectReferenceExt", GetNumItemsOfType, a_registry));

		a_registry->RegisterFunction(
			new NativeFunction3<StaticFunctionTag, TESForm*, TESObjectREFR*, UInt32, UInt32>("GetNthFormOfType", "iEquip_ObjectReferenceExt", GetNthFormOfType, a_registry));

		return true;
	}
}
