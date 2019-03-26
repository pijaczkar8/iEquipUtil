#pragma once

#include "GameAPI.h"  // g_thePlayer
#include "GameBSExtraData.h"  // BaseExtraList
#include "GameExtraData.h"  // InventoryEntryData, ExtraContainerChanges

#include "function_view.h"  // function_view

#include "RE/BSTList.h"  // BSSimpleList


inline void ForEachInvEntry(ext::function_view<bool(InventoryEntryData*)> a_fn)
{
	auto xChanges = static_cast<ExtraContainerChanges*>((*g_thePlayer)->extraData.GetByType(kExtraData_ContainerChanges));
	if (!xChanges || !xChanges->data || !xChanges->data->objList) {
		return;
	}

	auto invList = reinterpret_cast<RE::BSSimpleList<InventoryEntryData*>&>(*xChanges->data->objList);
	for (auto& item : invList) {
		if (!a_fn(item)) {
			break;
		}
	}
	return;
}


inline void ForEachExtraList(InventoryEntryData* a_entryData, ext::function_view<bool(BaseExtraList*)> a_fn)
{
	auto xLists = reinterpret_cast<RE::BSSimpleList<BaseExtraList*>&>(*a_entryData->extendDataList);
	for (auto& xList : xLists) {
		if (!a_fn(xList)) {
			break;
		}
	}
}
